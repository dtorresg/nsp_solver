#ifndef SECCOVER_H
#define SECCOVER_H

#include <string>
#include <vector>

using namespace std;

class SecCover {
    public:
        int day;
        string shiftID;
        int req;
        int under;
        int over;

        SecCover(int a, string b, int c, int d, int e) {
            day = a;
            shiftID = b;
            req = c;
            under = d;
            over = e;
        }
};

#endif