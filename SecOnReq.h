#ifndef SECONREQ_H
#define SECONREQ_H

#include <string>
#include <vector>

using namespace std;

class SecOnReq {
    public:
        string empId;
        int day;
        string shiftId;
        int weigth;

        SecOnReq(string a,int b, string c, int d){
            empId = a;
            day = b;
            shiftId = c;
            weigth = d;
        }
};

#endif