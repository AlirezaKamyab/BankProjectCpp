#ifndef TIME_H
#define TIME_H

#include <exception>
#include <string>
using namespace std;

class TimeException : public exception {
public:
    TimeException() : message{""} {}
    explicit TimeException(const string& msg) : message{msg} {}
    explicit TimeException(const char* msg) : message{msg} {}
    virtual ~TimeException() {}

    virtual const char* what(void) const noexcept { return message.c_str(); }
private:
    string message;
};

class Time {
public:
    Time(const int&, const int& = 0, const int& = 0);
    Time(const Time&);
    Time(Time&&) noexcept;
    ~Time();

    int getHour() const;
    int getMinute() const;
    int getSecond() const;
    void setHour(const int&);
    void setMinute(const int&);
    void setSecond(const int&);

    Time& operator=(const Time&);
    bool operator==(const Time&) const;
    operator std::string() const;
private:
    int _s;
    int _m; 
    int _h;

    Time();
    void reset();
};

#endif // TIME_H