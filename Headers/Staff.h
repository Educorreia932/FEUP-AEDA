#ifndef SUP_SCHOOL_STAFF_H
#define SUP_SCHOOL_STAFF_H

#include "Activity.h"

#include <vector>


class Teacher {
    public:
        Teacher();
        Teacher(string name);

        void setID(const unsigned int id);
        unsigned getID()const ;
        static unsigned int getLastID( );
        static void setLastID(const unsigned int id);
        void setName(string name);
        void addActivity(Activity* activity);
        bool isOcuppied(const Time startTime, const Time endTime);


        friend ostream& operator<<(ostream& out, const Teacher& C);
    private:
        string name;
        unsigned int id;
        static unsigned int last_id;
        vector<Activity *> Activities;


};

class teacherHasActivityAtSameTime : std::exception {
public:
    unsigned teacherId;
    unsigned int activityId;
    teacherHasActivityAtSameTime(unsigned int teacherId,unsigned int activityId){teacherId = teacherId;activityId = activityId;};
};

std::ostream & operator <<(std::ostream &out,const teacherHasActivityAtSameTime &ids);

class teacherAlreadHasActivity : std::exception {
public:
    unsigned teacherId;
    unsigned int activityId;
    teacherAlreadHasActivity(unsigned int teacherId,unsigned int activityId){teacherId = teacherId;activityId = activityId;};
};

std::ostream & operator <<(std::ostream &out,const teacherAlreadHasActivity &ids);


#endif //SUP_SCHOOL_STAFF_H
