#ifndef DATE_H
#define DATE_H
#include <iostream>
#include <stdexcept>
#include <string>
using namespace std;

class Date {
public:
    Date();
    Date(int d, int m, int y);
    Date(const string&);
    Date(const Date&);
    Date(Date&&) noexcept;
    ~Date();
    void set_date(int d, int m, int y);
    void print_date() const;
    bool equals(const Date& d) const;

    int get_day() const { return day; }
    int get_month() const { return month; }
    int get_year() const { return year; }
	void inc_one_day();
    int compare(const Date&) const;
    static bool is_leap_year(int year);
    static int max_day_for_month(int m, int y);
    static int days_between(const Date& d1, const Date& d2);
    static Date str_to_date(string s);

    Date& operator=(const Date&);
    bool operator==(const Date&) const;
    bool operator<(const Date&) const;
    bool operator>(const Date&) const;
    bool operator<=(const Date&) const;
    bool operator>=(const Date&) const;
    Date operator++(int);
    Date& operator++(void);

private:
    int day;
    int month;
    int year;

    void reset();
};

// Non-member functions
ostream& operator<<(ostream& o, const Date& dte);

#endif // DATE_H