#ifndef SECOFFREQ_H
#define SECOFFREQ_H

#include <string>
#include <vector>

using namespace std;

class SecOffReq {
    public:
        string empId;
        int day;
        string shiftId;
        int weigth;

        SecOffReq(string a,int b, string c, int d){
            empId = a;
            day = b;
            shiftId = c;
            weigth = d;
        }
};

#endif