#ifndef SECSHIFT_H
#define SECSHIFT_H

#include <string>
#include <vector>

using namespace std;

class SecShift {
    public:
        string name;
        int minutes;
        vector<string> notfollowthis;

        SecShift(string nombre, int tiempo, vector<string> notfoll) {
            name = nombre;
            minutes = tiempo;
            notfollowthis = notfoll;
        }
};

#endif