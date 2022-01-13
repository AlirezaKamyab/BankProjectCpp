#ifndef HELPERCLASS_H
#define HELPERCLASS_H

#include "date.h"
#include "time.h"
#include "ctime"

class Helper {
    static Time getCurrentTime();
    static Date getCurrentDate();
};

#endif // HELPERCLASS_H