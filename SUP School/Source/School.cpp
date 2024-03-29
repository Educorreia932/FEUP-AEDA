#include "../Headers/School.h"
#include "../Headers/Menu.h"

//Price of gold card
double School::goldCardPrice = 40;

School::School() {

}

School::School(const string& filename, unsigned int id) {
    string line;
    ifstream File(filename);
    int counter = 0;

    this->id = id;

    if (File.is_open()) {
        while (getline(File, line)) {
            switch (counter) {
                case 0:
                    name = line;
                    break;
                case 1:
                    locality = line;
                    break;
                case 2:
                    director = line;
                    break;
                case 3:
                    currentTime = Time(line);
                    break;
                case 4:
                    Files["Clients"] = line;
                    break;
                case 5:
                    Files["Materials"] = line;
                    break;
                case 6:
                    Files["Activities"] = line;
                    break;
                case 7:
                    Files["Teachers"] = line;
                    break;
                default:
                    break;
            }

            counter++;
        }

        File.close();

        readActivities();
        readClients();
        readMaterials();
        readTeachers();
    }

    else
        cerr << "ERROR: Couldn't read file with name " << filename << endl;
}

void School::removeClient(unsigned int id) {
    if(Clients[clientIndex(id)]->getId() == -1)
        throw NonExistentClient(id);

    for(auto &m: Materials) {
        auto it = m->Clients.find(Clients[clientIndex(id)]);
        m->Clients.erase(it);
    }

    for(size_t i = 0; i < Clients.size() ; i++)
        if (Clients.at(i)->getId() == id) {
            Clients.erase(Clients.begin()+i);
            break;
        }
}

void School::removeMaterial(unsigned int id) {
    if(Materials[materialIndex(id)]->getID() == -1)
        throw NonExistentMaterial(id);

    for(size_t i = 0; i < Materials.size() ; i++)
        if (Materials.at(i)->getID() == id) {
            Materials.erase(Materials.begin()+i);
            break;
        }
}

void School::addClient(Client* client) {
    if (clientIndex(client->getId()) != -1)
        throw ClientAlreadyExists(client->getId());

    Clients.push_back(client);
}

int School::clientIndex(unsigned int id) {
    for (size_t i = 0; i < Clients.size();i++){
        if (Clients.at(i)->getId() == id){
            return i;
        }
    }

    return -1;
}

int School::materialIndex(unsigned int id) {
    for (size_t i = 0; i < Materials.size();i++){
        if (Materials.at(i)->getID() == id){
            return i;
        }
    }

    return -1;
}

string School::getName() const {
    return name;
}

vector<Client *> School::getClients() const{
    return this->Clients;
}

TeacherHashTable School::getTeachers() const{
    return this->Teachers;
}

vector<Material *> School::getMaterials() const{
    return this->Materials;
}

void School::readActivities() {
    static int a = 0;
    a++;
    string line;
    ifstream File("../Data/" + to_string(id) + "/" + Files["Activities"]);
    int counter = 0, activity_id;

    //Things for the new activity
    int id;
    string type;
    string acName;
    string startTime;
    string endTime;

    if (File.is_open()) {
        while (getline(File, line)) {
            switch (counter % 7) {
                case 0:
                    id = stoul(line);
                    break;
                case 1:
                    type = line;
                    break;
                case 2:
                    acName = line;
                    break;
                case 3:
                    startTime = line;
                    break;
                case 4:
                    endTime = line;
                    break;
                case 5:
                    //Check if it's a lesson "L" or ride "R"
                    if(type == "L") {
                        auto* AuxLesson = new Lesson(id);
                        //AuxLesson->setID(id);
                        AuxLesson->setName(acName);
                        AuxLesson->setStartTime(startTime);
                        AuxLesson->setEndTime(endTime);

                        if (AuxLesson->getStartTime() < currentTime)
                            PastActivities.push_back(AuxLesson);
                        else
                            ScheduledActivities.push_back((AuxLesson));
                    }

                    else if (type == "R"){
                        Ride* AuxActivity = new Ride(id);
                        //AuxActivity->setID(id);
                        AuxActivity->setName(acName);
                        AuxActivity->setStartTime(startTime);
                        AuxActivity->setEndTime(endTime);
                        AuxActivity->setCost(stoul(line));

                        if (AuxActivity->getStartTime() < currentTime)
                            PastActivities.push_back(AuxActivity);

                        else
                            ScheduledActivities.push_back((AuxActivity));
                    }

                    else if (type == "F"){
                        cout << a << endl;
                        Fixing * auxFixing = new Fixing(id);
                        auxFixing->setName(acName);
                        auxFixing->setStartTime(startTime);
                        auxFixing->setEndTime(endTime);

                        if (auxFixing->getStartTime() < currentTime)
                            PastFixes.push_back(auxFixing);
                        else
                            ScheduledFixes.push_back((auxFixing));
                    }

                    else
                        cerr << "Non activity, ride or repair found whilst reading activities" << endl;

                    break;
            }

            counter++;
        }
    }
}

void School::readClients() {
    string line;
    ifstream File("../Data/" + to_string(id) + "/" + Files["Clients"]);
    int counter = 0;
    auto* auxClient = new Client();
    auto* activities = new stringstream;

    if (File.is_open()) {
        while (getline(File, line)) {
                switch (counter % 6) {
                    case 0: // Name
                        auxClient->setName(line);
                        break;
                    case 1: // ID
                        auxClient->setID((stoi(line)));

                        if (stoi(line) > Client::getLastID())
                            Client::setLastID(stoi(line));

                        break;
                    case 2: // Gold membership
                        auxClient->setGoldMember(stob(line));
                        break;
                    case 3: // Balance
                        auxClient->addBalance(stod(line));
                        break;
                    case 4: // Activities
                        *activities << line;
                        break;
                    case 5:
                        Clients.push_back(auxClient);

                        readClientsActivities(activities, auxClient);

                        activities->clear();
                        auxClient = new Client();
                        break;
            }

            counter++;
        }

        File.close();
    }
}

void School::readTeachers() {
    string line;
    ifstream File("../Data/" + to_string(id) + "/" + Files["Teachers"]);
    int counter = 0;
    auto* auxTeacher = new Teacher();
    auto* activities = new stringstream;

    string name;
    unsigned int id;

    if(File.is_open()){
        while(getline(File, line)) {
            if (counter % 4 == 0) {
                name = line;
                counter++;
            }

            else if (counter % 4 == 1) {
                id = stoul(line);
                counter++;
            }

            else if (counter % 4 == 2) {
                if (line == "T") {
                    //printf("Read teachers not done for technicians.");

                    auto* auxTech = new Technician();
                    auxTech->setName(name);
                    auxTech->setID(id);

                    line.replace(0, 1, "");
                    *activities << line;

                    //readTechniciansActivities(activities, auxTech);
                    Technicians.push(auxTech);
                    activities->clear();
                    counter += 2;
                }

                else {
                    activities = new stringstream(line);

                    counter++;
                }
            }

            else if (counter % 4 == 3) {
                auxTeacher->setName(name);
                auxTeacher->setID(id);
                auxTeacher->setCurrentlyEmployed(stob(line));

                Teachers.insert(auxTeacher);

                readTeachersActivities(activities, auxTeacher); ///ERROR

                getline(File, line);

                auxTeacher = new Teacher();

                counter++;
            }
        }
    }

    File.close();
}

void School::readMaterials() {
    string line;
    ifstream File("../Data/1/" + Files["Materials"]);
    int counter = 0;
    map<Client*,vector<Time>> client_map;
    unsigned client_id;
    Time startTime;
    Time endTime;

    auto* activities = new stringstream;
    auto* clients = new stringstream;

    bool leaveCase = false;

    string type;
    unsigned id;

    if (File.is_open()) {
        while (getline(File, line)) {
                switch (counter % 5) {
                    case 0:
                        type = line;
                        break;
                    case 1:
                        id = stoi(line);
                        break;
                    case 2:
                        *activities << line;
                        break;
                    case 3:
                        if (line == "::::::::::" || line == "---END---") {
                            leaveCase = true;
                            counter++;
                        } else {
                            *clients << line;
                            *clients >> client_id;
                            string auxgdfgd = clients->str().substr(2, 16);
                            string auxgdfghfghgd = clients->str().substr(19, 16);
                            startTime = Time(clients->str().substr(2, 16));
                            endTime = Time(clients->str().substr(19, 16));

                            client_map[Clients[clientIndex(client_id)]] = {startTime, endTime};
                            clients->clear();
                        }
                        if (!leaveCase) {
                            while (getline(File, line)) {
                                if (line == "::::::::::" || line == "---END---") {
                                    leaveCase = true;
                                    counter++;
                                    break;
                                } else {
                                    *clients << line;
                                    *clients >> client_id;
                                    startTime = Time(clients->str().substr(0, 16));
                                    endTime = Time(clients->str().substr(17, 16));

                                    client_map[Clients[clientIndex(client_id)]] = {startTime, endTime};
                                    clients->clear();
                                }
                            }
                        }




                    case 4:
                        if (type == "boat") {
                            Boat *auxBoat = new Boat();
                            auxBoat->setType(type);
                            auxBoat->setID(id);

                            if (id > Material::getLastID())
                                Material::setLastID(id);


                            readMaterialActivities(activities, auxBoat);
                            auxBoat->setClients(client_map);
                            Materials.push_back(auxBoat);
                        } else if (type == "suits") {
                            Suits *auxSuits = new Suits();
                            auxSuits->setType(type);
                            auxSuits->setID(id);

                            if (id > Material::getLastID())
                                Material::setLastID(id);


                            readMaterialActivities(activities, auxSuits);
                            auxSuits->setClients(client_map);
                            Materials.push_back(auxSuits);
                        } else if (type == "board") {
                            Board *auxBoard = new Board();
                            auxBoard->setType(type);
                            auxBoard->setID(id);

                            if (id > Material::getLastID())
                                Material::setLastID(id);


                            readMaterialActivities(activities, auxBoard);
                            auxBoard->setClients(client_map);
                            Materials.push_back(auxBoard);
                        }

                        activities->clear();
                        client_map.clear();
                        break;
                }
            leaveCase = false;
            counter++;
            }

        }

        File.close();
}

void School::enroll(const unsigned int clientId, const unsigned int activityId) {
    //Time needs to be checked if is ahead of the set current time

    Client* client;
    bool clientExists = false;

    for (const auto &c : this->Clients) {
        if(c->getId() == clientId){
            client = c;
            clientExists = true;
            break;
        }
    }

    if(!clientExists)
        throw NonExistentClient(clientId);

    bool activityExists = false;

    for (const auto &ac : ScheduledActivities) {
        if (activityId == ac->getId()){
            try {
                client->addActivity(ac, false);
                activityExists = true;
            }

            catch (clientAlreadHasActivity &e) {
                throw e;
            }
        }
    }

    if(!activityExists)
        throw activityNonExistent(activityId);
}

void School::assign(const unsigned int teacherId, const unsigned int activityId) {
    //Time needs to be checked if is ahead of the set current time

    Teacher* teacher;


    bool teacherExists = false;

    for (const auto &t : this->Teachers) {
        if(t->getID() == teacherId){
            teacher = t;
            teacherExists = true;
            break;
        }
    }

    if(!teacherExists)
        throw NonExistentTeacher(teacherId);

    Teachers.erase(teacher);

    bool activityExists = false;

    for (const auto &ac : ScheduledActivities) {
        if (activityId == ac->getId()){
            try {
                teacher->addActivity(ac, false);
                activityExists = true;
            }

            catch (teacherAlreadHasActivity &e) {
                throw e;
            }
            catch(teacherHasActivityAtSameTime &e){
                throw e;
            }
        }
    }

    Teachers.insert(teacher);

    if(!activityExists)
        throw activityNonExistent(activityId);
}

void School::readClientsActivities(stringstream* activities, Client* c) {
    int activity_id;

    while (*activities >> activity_id) {
        Activity* AuxActivity = getActivity(activity_id);

        if (AuxActivity->getEndTime() < currentTime)
            c->addActivity(AuxActivity, true);

        else {
            try {
                enroll(c->getId(), activity_id);
            }

            catch (activityNonExistent &e) {
                cout << e;
                Menu::pause();
            }

            catch (hasActivityAtSameTime &e) {
                cout << e;
                Menu::pause();
            }

            catch (clientAlreadHasActivity &e) {
                cout << e;
                Menu::pause();
            }
        }
    }
}

void School::readTeachersActivities(stringstream* activities, Teacher* t) {
    int activity_id;

    while (*activities >> activity_id) {
        Activity* AuxActivity = getActivity(activity_id);

        if (AuxActivity->getEndTime() < currentTime) {
            Teachers.erase(t);
            t->addActivity(AuxActivity, true);
            Teachers.insert(t);
        }

        else {
            try {
                assign(t->getID(), activity_id);
            }

            catch (activityNonExistent &e) {
                cout << e;
                Menu::pause();
            }

            catch (teacherHasActivityAtSameTime &e) {
                cout << e;
                Menu::pause();
            }

            catch (teacherAlreadHasActivity &e) {
                cout << e;
                Menu::pause();
            }
        }
    }
}

void School::readMaterialActivities(stringstream* activities, Material* m) {
    int activity_id;

    while (*activities >> activity_id) {
        Activity* AuxActivity = getActivity(activity_id);

        if (AuxActivity->getEndTime() < currentTime)
            m->addActivity(AuxActivity);
    }
}

ostream &operator<<(ostream &out, const School& S) {
    out << "Name: " << S.name << endl
        << "Director" << S.director << endl
        << "Current date: " << S.currentTime << endl
        << "Number of enrolled clients: " << S.Clients.size() << endl
        << "Number of occurred activities: " << S.PastActivities.size() << endl
        << "Number of planned activities: " << S.ScheduledActivities.size() << endl;

    return out;
}

void School::viewClients(bool detailed) {
    if (detailed)
        for (auto & Client : Clients)
            cout << *Client << endl
                 << "---------------------" << endl;

    else
        for (auto & Client : Clients) {
            cout << Client->getName() << " - " << Client->getId();

            if (Client->isGoldMember())
                cout << " ($)";

            cout << endl;
        }
}

void School::viewMaterial(bool detailed){
    if (detailed)
        for (auto & Material : Materials)
            cout << *Material
                 << "---------------------" << endl;

    else
        for (auto & Material : Materials) {
            cout << Material->getType() << " - " << Material->getID() << endl;
        }
}

void School::viewActivities(bool detailed) {
    if (detailed) {
        cout << "Scheduled Activities:\n";
        cout << "---------------------" << endl;

        for (const auto &Activity : ScheduledActivities) {
            cout << *Activity;
            cout << "---------------------" << endl;
        }
    }

    else
        for (auto & activity : ScheduledActivities)
            cout << activity->getName() << " - " << activity->getId() << endl;
}

void School::viewFixes(bool detailed){
    if (detailed) {
        cout << "Scheduled Repairs:\n";
        cout << "---------------------" << endl;

        for (const auto &Fixing : ScheduledFixes) {
            cout << *Fixing;
            cout << "---------------------" << endl;
        }
    }
}

bool sortRule(Teacher * t1, Teacher * t2) {
    return t1->getID() < t2->getID();
}

void School::viewTeachers(bool detailed) {
    vector<Teacher*> auxVec;
    for (auto & teacher : Teachers){
        auxVec.push_back(teacher);
    }
    sort(auxVec.begin(),auxVec.end(),sortRule);

    if (detailed)
        for (auto & teacher : auxVec)
            cout << *teacher
                 << "---------------------" << endl;

    else
        for (auto & teacher : auxVec)
            cout << teacher->getName() << " - " << teacher->getID() << endl;
}

ostream &operator<<(ostream &out, const Technician &T) {
    out << "ID: " << T.getID() << endl;
    out << "Name: " << T.getName() << endl;

    if (T.ScheduledFixes.empty())
        out << "No fixes associated";

    else {
        out << "Scheduled fixes: ";

        for (auto a : T.ScheduledFixes)
            out << a->getId() << " ";
    }

    out << endl;

    return out;
}

void School::viewTechnicians(bool detailed){
    priority_queue<Technician *, vector<Technician *>, TechnicianCompare> aux = Technicians;
    if (detailed) {
        while(!aux.empty()){
            cout << *(aux.top())
                 << "---------------------" << endl;
            aux.pop();
        }
    }
    else
        while(!aux.empty()){
            cout << aux.top()->getName() << " - " << aux.top()->getID() << endl;
            aux.pop();
        }
}

void School::saveActivities() {
    ofstream f;
    int counter = 0;

    f.open("../Data/" + Files["Activities"]);
    vector<Activity*> Activities;
    Activities.reserve(PastActivities.size() + ScheduledActivities.size());
    Activities.insert(Activities.end(), PastActivities.begin(), PastActivities.end());
    Activities.insert(Activities.end(), ScheduledActivities.begin(), ScheduledActivities. end());
    sort(Activities.begin(), Activities.end());
    if (f.is_open())
        for (auto c : Activities) {
            f << c->getId() << endl
              << c->getType() << endl
              << c->getName() << endl
              << c->getStartTime() << endl
              << c->getEndTime() << endl
              << c->CalcCost() << endl;

            if (counter == size(Activities) - 1)
                f << "---END---";

            else
                f << "::::::::::" << endl;

            counter++;
        }

    f.close();
}

void School::saveClients() {
    ofstream f;
    int counter = 0;

    f.open("../Data/" + Files["Clients"]);

    if (f.is_open())
        for (auto c : Clients) {
            f << c->getName() << endl
              << c->getId() << endl
              << btos(c->getGoldMember()) << endl
              << c->getBalance() << endl
              << c->getPastActivitiesID() << c->getScheduledActivitiesID() << endl;

            if (counter == size(Clients) - 1)
                f << "---END---";

            else
                f << "::::::::::" << endl;

            counter++;
        }

    f.close();
}

void School::saveMaterials() {
    ofstream f;
    int counter = 0;

    f.open("../Data/" + Files["Materials"]);

    if (f.is_open())
        for (auto m : Materials) {
            f << m->getType() << endl
              << m->getID() << endl
              << m->getActivitiesID() << endl;
            for (const auto &client: m->Clients){
                f << client.first->getId() << " " << client.second[0] << " " << client.second[1] << endl;
            }


            if (counter == size(Materials) - 1)
                f << "---END---";
            else
                f << "::::::::::" << endl;

            counter++;
        }

    f.close();
}

vector<Time> School::getDatesFromActivicties(vector <Activity*> Activities) {
    vector<Time> Dates;
    Time auxDate;
    bool already_present = false;

    for (auto a : Activities) {
         auxDate = Time(a->getStartTime().getDay(), a->getStartTime().getMonth(), a->getStartTime().getYear());

         for (auto d : Dates) // See if it's already in the vector
             if (d == auxDate)
                already_present = true;

         if (!already_present)
             Dates.push_back(auxDate);

         already_present = false;
    }

    return Dates;
}

void School::viewDates(vector <Time> Dates) {
    int counter = 1;

    for (auto d : Dates) {
        cout << counter << ") " << d.toString() << endl;
        counter++;
    }

    cout << "0) Go back." << endl;
}

void School::saveTeachers() {
    ofstream f;
    int counter = 0;

    f.open("../Data/" + Files["Teachers"]);

    if (f.is_open())
        for (auto t : Teachers) {
            f << t->getName() << endl
              << t->getID() << endl
              << t->getPastActivitiesID() << t->getScheduledActivitiesID() << endl;

            if(t->getCurrentlyEmployed()){
                f << "true" << endl;
            } else {
                f << "false" << endl;
            }

            if (counter == size(Teachers) - 1)
                f << "---END---";

            else
                f << "::::::::::" << endl;

            counter++;
        }

    f.close();
}

void School::addTeacher(Teacher* teacher) {

    for(const auto &t : Teachers){
        if(t->getID() == teacher->getID()){
            throw TeacherAlreadyExists(teacher->getID());
        }
    }

    Teachers.insert(teacher);
}

/*
int School::teacherIndex(unsigned int id) {


    for (size_t i = 0; i < Teachers.size();i++){
        if (Teachers.at(i)->getID() == id){
            return i;
        }
    }

    return -1;
}
 */

void School::addActivity(Activity* activity, bool past) {
    if (past)
        PastActivities.push_back(activity);
    else
        ScheduledActivities.push_back(activity);
}

void School::addFixing(Fixing* fixing, bool past) {
    if (past)
        PastFixes.push_back(fixing);

    else
        ScheduledFixes.push_back(fixing);
}

void School::removeTeacher(unsigned id) {

    Teacher* auxT;
    bool found = false;

    for(const auto &t : Teachers){
        if(t->getID() == id){
            auxT = t;
            found = true;
            break;
        }
    }

    Teachers.erase(auxT);
    auxT->setCurrentlyEmployed(false);
    Teachers.insert(auxT);

    if (!found)
        throw NonExistentTeacher(id);

}

int School::activityIndex(unsigned int id, bool past) {
    if (past) {
        for (size_t i = 0; i < PastActivities.size();i++)
            if (PastActivities.at(i)->getId() == id)
                return i;
    }

    else {
        for (size_t i = 0; i < ScheduledActivities.size();i++)
            if (ScheduledActivities.at(i)->getId() == id)
                return i;
    }

    return -1;
}

Activity * School::getActivity(unsigned int id) const {
    for (auto a : PastActivities)
        if (a->getId() == id)
            return a;

    for (auto a : ScheduledActivities)
        if (a->getId() == id)
            return a;

    return nullptr;
}

void School::rent(const unsigned int materialId, const unsigned int clientId, Time startTime, Time endTime) {

    if(clientIndex(clientId) == -1)
        throw NonExistentClient(clientId);

    if (materialIndex(materialId) == -1)
        throw NonExistentMaterial(materialId);


    if(Materials[materialIndex(materialId)]->beingUsed(startTime,endTime))
        throw alreadyInUse(materialId,startTime,endTime);

    try{
        if(Materials[materialIndex(materialId)]->getType() == "boat")
            Clients[clientIndex(clientId)]->addBalance(-Boat::cost);
        else if (Materials[materialIndex(materialId)]->getType() == "suits")
            Clients[clientIndex(clientId)]->addBalance(-Suits::cost);
        else if (Materials[materialIndex(materialId)]->getType() == "board")
            Clients[clientIndex(clientId)]->addBalance(-Board::cost);
    } catch (insufficientFunds &e){
        cerr << e;
    }

    Materials[materialIndex(materialId)]->Clients[Clients[clientIndex(clientId)]] = {startTime,endTime};
}

string School::getLocality() const {
    return locality;
}

bool operator <(const School s1, const School s2)  {
    if (s1.getClients().size() == s2.getClients().size())
        return s1.getLocality() < s2.getLocality();

    return s1.getClients().size() < s2.getClients().size();
}

priority_queue<Technician *, vector<Technician *>, TechnicianCompare> School::GetTechnicians() const{
    return Technicians;
}

//Exceptions

ostream &operator<<(ostream &out, const NonExistentClient &client) {
    out << "Client with ID " << client.id << " does not exist in school." << endl;
    return out;
}

ostream &operator<<(ostream &out, const ClientAlreadyExists &client) {
    out << "Client with ID " << client.id << " already exists in school." << endl;
    return out;
}

ostream &operator<<(ostream &out, const NonExistentTeacher &teacher) {
    out << "Teacher with ID " << teacher.id << " does not exist in school." << endl;
    return out;
}

ostream &operator<<(ostream &out, const TeacherAlreadyExists &teacher) {
    out << "Teacher with ID " << teacher.id << "already exists in school." << endl;
    return out;
}

ostream &operator<<(ostream &out, const NonExistentMaterial &material) {
    out << "Material with ID " << material.id << " does not exist in school." << endl;
    return out;
}
