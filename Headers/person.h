#ifndef PERSON_H
#define PERSON_H

#include "date.h"
#include <vector>
using namespace std;

class PersonException : public exception {
public:
    PersonException() : message{""} {}
    explicit PersonException(const string& msg) : message{msg} {}
    explicit PersonException(const char* msg) : message{msg} {}
    virtual ~PersonException() {}

    virtual const char* what(void) const noexcept { return message.c_str(); }
private:
    string message;
};

class Person {
public:
    Person(const string&, const string&, const string&, const Date&);
    Person(const Person&);
    Person(Person&&) noexcept;
    virtual ~Person();

    string getName() const;
    string getLastName() const;
    string getId() const;
    Date getBirthday() const;

    Person& operator=(const Person& rhs);
    bool operator==(const Person& rhs) const;
    bool operator!=(const Person& rhs) const;
    operator string() const;

protected:
    string _name;
    string _lastname;
    string _id;
    Date _birthday;
    
    Person();
    void reset();

private:
    static vector<string> _personIds;
};

#endif // PERSON_H