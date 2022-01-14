#include "../Headers/person.h"
#include <sstream>

vector<string> Person::_personIds;

Person::Person() {}

Person::Person(const string& name, const string& lastname, const string& id, const Date& bdate) : _name{name},
    _lastname{lastname}, _id{id}, _birthday{bdate} {
        for(string str : _personIds) {
            if(str == id) {
                throw PersonException{"Duplicate person with the same id spotted!"};
            }
        }
        _personIds.push_back(id);
    }

Person::Person(const Person& other) {
    _name = other._name;
    _lastname = other._lastname;
    _id = other._id;
    _birthday = other._birthday;
}

Person::Person(Person&& other) noexcept : Person{other} {
    other.reset();
}

Person::~Person() {}

string Person::getName() const { return _name; }
string Person::getLastName() const { return _lastname; }
string Person::getId() const { return _id; }
Date Person::getBirthday() const { return _birthday; }
void Person::reset() {
    _name = "";
    _lastname = "";
    _id = "";
    _birthday = Date{1,1,1390};
}

Person& Person::operator=(const Person& rhs) {
    if(&rhs == this) return *this;
    _name = rhs._name;
    _lastname = rhs._lastname;
    _id = rhs._id;
    _birthday = rhs._birthday;
    
    return *this;
}

bool Person::operator==(const Person& rhs) const {
    return _id == rhs._id;
}

bool Person::operator!=(const Person& rhs) const {
    return !(*this == rhs);
}

Person::operator std::string() const {
    stringstream str;
    str << _name << " " << _lastname << endl;
    str << "ID: " << _id << endl;
    str << "Birthday: " << _birthday << endl;
    return str.str();
}