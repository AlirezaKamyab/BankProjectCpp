#include "../Headers/helperClass.h"

Time Helper::getCurrentTime() {
    time_t t = time(0);
    tm* now = localtime(&t);
    Time temp {now->tm_hour, now->tm_min, now->tm_sec};
    return temp;
}

Date Helper::getCurrentDate() {
    time_t t = time(0);
    tm* now = localtime(&t);
    Date temp {now->tm_mday, now->tm_mon, now->tm_year};
}

static string generateRandom(const int& size) {
    static bool setSeed = false;
    if(!setSeed) {
        setSeed = true;
        srand(time(0));
    }

    string n;

    for(int i = 0; i < size; i++) {
        int digit = rand() % 10;
        n += digit + '0';
    }

    return n;
}