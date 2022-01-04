#ifndef PERSON_H
#define PERSON_H

#include "date.h"
using namespace std;

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
};

#endif // PERSON_H