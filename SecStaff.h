#ifndef SECSTAFF_H
#define SECSTAFF_H

#include <string>
#include <vector>

using namespace std;

class SecStaff {
    public:
        string id;
        vector<int> MaxShifts;
        int MaxTM;
        int MinTM;
        int MaxCS;
        int MinCS;
        int MinCDO;
        int MaxW;

        SecStaff(string a, vector<int> b, int c, int d,int e,int f,int g,int h){
            id = a;
            MaxShifts = b;
            MaxTM = c;
            MinTM = d;
            MaxCS = e;
            MinCS = f;
            MinCDO = g;
            MaxW = h;
        }
};

#endif