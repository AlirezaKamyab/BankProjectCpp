#ifndef HELPERCLASS_H
#define HELPERCLASS_H

#include "date.h"
#include "time.h"
#include "ctime"
#include "random"


class Helper {
    static Time getCurrentTime();
    static Date getCurrentDate();
    static string generateRandom(const int&);
};

#endif // HELPERCLASS_H