#include "../Headers/Auxiliary.h"

int readOption(int min, unsigned int max) {
    int option;

    while (true) {
        cout << "Your option: ";

        if (cin >> option && option >= min && option <= max) {
            cin.ignore(1000, '\n');
            return option;
        }

        else {
            cin.clear();
            cin.ignore(1000, '\n');
            cerr << endl
                 << "Invalid input. Please try again:" << endl
                 << endl;
        }
    }
}

double readOption(double min, double max) {
    double option;

    while (true) {
        cout << "Your option: ";

        if (cin >> option && option >= min && option <= max) {
            cin.ignore(1000, '\n');
            return option;
        }

        else {
            cin.clear();
            cin.ignore(1000, '\n');
            cerr << endl
                 << "Invalid input. Please try again:" << endl
                 << endl;
        }
    }
}

bool stob(const string& s) {
    return s == "true";
}

string centerString(int width, const string& s) {
    int length = s.length();
    int pos = (width - length) / 2;
    string result = string(pos, ' ') + s + string(width - pos - length, ' ');

    return result;
}
/*
template <class T>
vector<T*> eraseAndReturnVector(vector<T*> vec,unsigned int i){
    erase(vec, i);
    return vec;
}
*/

string btos(const bool &b) {
    if (b)
        return "true";

    return "false";
};

