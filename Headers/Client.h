#ifndef SUP_SCHOOL_CLIENT_H
#define SUP_SCHOOL_CLIENT_H

#include <fstream>
#include <string>
#include <vector>

#include "Activity.h"
#include "Auxiliary.h"

class Client {
    private:
        vector<Activity*> PastActivities;
        vector<Activity*> ScheduledActivities;
        static unsigned int id;
        string name;
        bool gold_member;
    public:
        //Constructors
        Client();
        Client(string name, bool has_gold_card);

        //Getters
        string getName() const;
        unsigned int getId() const;

        //Setters
        void setName(const string newName);
        void setID(const unsigned int id);
        void setGoldMember(const bool gold_member);

        //Misc.
        void purchaseGold();
        bool isGoldMember() const;
        void enroll(const unsigned int activityId,const vector<Activity*> schoolActivities);
        bool isOcuppied(Time startTime,Time endTime); //Not implemented
};

class alreadyGoldMember : std::exception {
    public:
        unsigned int id;
        alreadyGoldMember(unsigned int id){id = id;};
};

std::ostream & operator <<(std::ostream &out,const alreadyGoldMember &member);


class hasActivityAtSameTime : std::exception {
public:
    unsigned clientId;
    unsigned int activityId;
    hasActivityAtSameTime(unsigned int clientId,unsigned int activityId){clientId = clientId;activityId = activityId;};
};

std::ostream & operator <<(std::ostream &out,const hasActivityAtSameTime &ids);

#endif //SUP_SCHOOL_CLIENT_H
