#include "../Headers/date.h"
#include <sstream>

Date::Date()
{
	//default constructor
    set_date(1, 1, 1390);
}

Date::Date(int d, int m, int y)
{
	//constructor
    set_date(d, m, y);
}


Date::Date(const string& str) {
    *this = Date::str_to_date(str);
}

Date::Date(const Date& cpy) {
    day = cpy.day;
    month = cpy.month;
    year = cpy.year;
}

Date::Date(Date&& mve) noexcept {
    year = move(mve.year);
    month = move(mve.month);
    day = move(mve.day);
    mve.reset();
}

Date::~Date() {
    reset();
}

void Date::reset() {
    year = 1390;
    month = 1;
    day = 1;
}

int Date::compare(const Date& other) const {
    if(year > other.year) return 1;
    else if(year < other.year) return -1;
    else {
        if(month > other.month) return 1;
        else if(month < other.month) return -1;
        else {
            if(day > other.day) return 1;
            else if(day < other.day) return -1;
            else return 0;
        }
    }
}

bool Date::is_leap_year(int year)
{
    int r = year % 33;
    return r==1 || r==5 || r==9 || r==13 || r==17 || r==22 || r==26 || r==30;
}

int Date::max_day_for_month(int m, int y)
{
    if (m < 7)
        return 31;
    else if (m < 12)
        return 30;
    else if (m == 12)
        return Date::is_leap_year(y) ? 30 : 29;
    else  
        throw invalid_argument("Invalid input for either year or month");
}

void Date::inc_one_day()
{
    day++;
    if (day > max_day_for_month(month, year)) {
        day = 1;
        month++;
        if (month > 12) {
            month = 1;
            year++;
        }
    }
}

void Date::set_date(int d, int m, int y)
{
    if (y < 0 || m < 1 || m > 12 || d < 1 || d > max_day_for_month(m, y))
        throw invalid_argument("Invalid arguments for either day, month or year");

    day = d;
    month = m;
    year = y;
}

void Date::print_date() const
{
	//baraye chape tarikh
    cout << day << '/' << month << '/' << year << endl;
}

bool Date::equals(const Date& d) const {
	//moghaieseye 2 tarikh
    return day == d.day && month == d.month && year == d.year;
}
int Date::days_between(const Date& d1, const Date& d2) {
    if(d1 < d2) {
        int count = 0;
        Date temp = d1;
        while (!temp.equals(d2)) {
            ++temp;
            count++;
        }
        return count;
    }
    else if(d1 > d2) return days_between(d2, d1);
    else return 0;
}

Date Date::str_to_date(string s) {
    for(char& c : s) {
        if(c == '/') c = ' ';
    }

    int d, m, y;
    stringstream stream{s};
    
    stream >> y;
    stream >> m;
    stream >> d;

    return Date(d, m, y);
}

Date& Date::operator=(const Date& rhs) {
    if(&rhs == this) return *this;
    day = rhs.day;
    month = rhs.month;
    year = rhs.year;
    return *this;
}

bool Date::operator==(const Date& rhs) const {
    return equals(rhs);
}

bool Date::operator<(const Date& rhs) const {
    if(compare(rhs) == -1) return true;
    else return false;
}

bool Date::operator>(const Date& rhs) const {
    if(compare(rhs) == 1) return true;
    else return false;
}

bool Date::operator<=(const Date& rhs) const {
    if(compare(rhs) < 1) return true;
    else return false;
}

bool Date::operator>=(const Date& rhs) const {
    if(compare(rhs) > -1) return true;
    else return false;
}

Date Date::operator++(int) {
    Date temp = *this;
    inc_one_day();
    return temp;
}

Date& Date::operator++(void) {
    inc_one_day();
    return *this;
}

// Non-member functions
ostream& operator<<(ostream& o, const Date& dte) {
    return o << dte.get_year() << "/" << dte.get_month() << "/" << dte.get_day();
}

istream& operator>>(istream& in, Date& dte) {
    string str;
    cin >> str;
    dte = Date::str_to_date(str);
    return in;
}
