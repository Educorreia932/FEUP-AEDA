#include "../Headers/Material.h"


using namespace std;
unsigned int Boat::maxCapacity = 10;
unsigned int Suits::maxCapacity = 6;
unsigned int Board::maxCapacity = 4;
unsigned int Material::last_id = 0;

bool Material::beingUsed(Time startTime, Time endTime){
    try{
        if (startTime > endTime) { //Implement >=
            throw ImpossibleTimeDifference(startTime, endTime);

        }
    }
    catch(ImpossibleTimeDifference &i){
        cerr << "Start time of " << i.getStartTime() << " is after end time of " << i.getEndTime() << endl;
    }

    if (Activities.size() == 0) //Or activity == NULL
        return true;

    for(int i = 0; i < Activities.size(); i++){
        //if(*(activity[i]))
        if(!(endTime < Activities[i]->getStartTime() || startTime > Activities[i]->getEndTime())){
             return true;
        }
    }

    return false;
}

ostream &operator<<(ostream &out, Material material) {
    out << "This is a" <<  material.getType() << ", which is going to be used by:\n";
    for(int i= 0; i < material.getType().size(); i++){
        cout << (material.getActivities())[i]->getName();
        if(i != material.getActivities().size() - 1){
            cout << ", ";
        }
    }
    return out;
}

//Setters
void Material::setID(int ID){
    this->ID = ID;
}

void Material::setType(string type){
    this->type = type;
}

void Material::setLastID(unsigned int id) {
    last_id = id;
}

void Material::setCapacity(unsigned int capacity){
    capacity = capacity;
}
//Getters
unsigned int Material::getID() const{
    return ID;
}

string Material::getType() const{
    return type;
}

vector<Activity *> Material::getActivities() const {
    return Activities;
}

unsigned int Material::getLastID(){
    return last_id;
}

unsigned int Material::getCapacity(){
    return capacity;
}
