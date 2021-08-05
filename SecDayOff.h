#ifndef SECDAYOFF_H
#define SECDAYOFF_H

#include <string>
#include <vector>

using namespace std;

class SecDayOff {
    public:
        string id;
        vector<int> DO;
        SecDayOff(string a, vector<int> Days){
            id = a;
            DO = Days;
        }
};

#endif