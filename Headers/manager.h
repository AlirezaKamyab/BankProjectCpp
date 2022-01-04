#ifndef MANAGER_H
#define MANAGER_H

#include "employee.h"
#include "exception"
using namespace std;

class ManagerException : public std::exception {
public:
    ManagerException() : message{""} {}
    explicit ManagerException(const string& msg) : message{msg} {}
    explicit ManagerException(const char* msg) : message{msg} {}
    virtual ~ManagerException() {}

    virtual const char* what() const noexcept { return message.c_str(); }
private:
    string message;
};

class Manager : public Employee {
public:
    Manager(const string&, const string&, const string&, const int&, const Date&, const string&, const string&);
    Manager(const string&, const string&, const string&, const int&, const Date&, const string&, const string&, 
        const int64_t&, const int&, const int&, Bank*);
    Manager(const Manager&);
    Manager(Manager&&) noexcept;
    virtual ~Manager();

    string employeeInfo(const int&) const; // should be implemented
    void hireEmployee(Employee*) const; // should be implemented
    void fireEmployee(const int&) const; // should be implemented

    Manager& operator=(const Manager&);
private:
    Manager();
    void reset();
};

#endif // MANAGER_H