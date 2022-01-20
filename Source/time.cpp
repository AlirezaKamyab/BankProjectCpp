#include "../Headers/time.h"
#include <sstream>
#include <iomanip>

Time::Time() : Time{0,0,0} {}
Time::Time(const int& hour, const int& minute, const int& second) {
    setHour(hour);
    setMinute(minute);
    setSecond(second);
}
Time::Time(const Time& other) : Time{other._h, other._m, other._s} {}
Time::Time(Time&& other) noexcept : Time{other} { other.reset(); } 
Time::~Time() {
    reset();
}

void Time::reset() {
    _h = 0;
    _m = 0;
    _s = 0;
}

int Time::getHour() const { return _h; }
int Time::getMinute() const { return _m; }
int Time::getSecond() const { return _s; }
void Time::setHour(const int& hour) {
    if(hour < 0 || hour > 23) throw TimeException{"Invalid argument for time"};
    _h = hour;
}
void Time::setMinute(const int& minute) {
    if(minute < 0 || minute > 59) throw TimeException{"Invalid argument for time"};
    _m = minute;
}
void Time::setSecond(const int& second) {
    if(second < 0 || second > 59) throw TimeException{"Invalid argument for time"};
    _s = second;
}

Time& Time::operator=(const Time& rhs) {
    if(&rhs == this) return *this;
    _h = rhs._h;
    _m = rhs._m;
    _s = rhs._s;
    return *this;
}
bool Time::operator==(const Time& rhs) const {
    return (_h == rhs._h && _m == rhs._m && _s == rhs._s);
}
Time::operator std::string() const {
    stringstream str;
    str << setw(0) << setfill('0') << _h;
    str << ":";
    str << setw(2) << _m;
    str << ":";
    str << setw(2) << _s;
    return str.str();
}

// Non-member functions
ostream& operator<<(ostream& out, const Time& time) {
    out << (string) time;
    return out;
}