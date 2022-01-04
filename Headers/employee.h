#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "person.h"
#include "client.h"
#include "user.h"
#include <string>
using namespace std;

class Bank;
class Account;

class EmployeeException : public exception {
public:
    EmployeeException() : message{""} {}
    explicit EmployeeException(const string& msg) : message{msg} {}
    explicit EmployeeException(const char* msg) : message{msg} {}
    virtual ~EmployeeException() {}

    virtual const char* what() const noexcept { return message.c_str(); }
private:
    string message;
};

class Employee : public User, public Person {
public:
    Employee(const string&, const string&, const string&, const int&, const Date&, const string&, const string&);
    Employee(const string&, const string&, const string&, const int&, const Date&, const string&, const string&, const int64_t&,
             const int&, const int&, Bank*);
    Employee(const Employee&);
    Employee(Employee&&) noexcept;
    virtual ~Employee();

    int getPersonnelId() const;
    Bank* getBank() const;
    int64_t getBaseIncome() const;
    int64_t getIncome() const;
    int64_t getPenalty() const;
    int64_t getReward() const;
    int getVacationHours() const;
    int getExtraHours() const;

    void setBaseIncome(const int64_t&);
    void setPenalty(const int64_t&);
    void setReward(const int64_t&);
    void setVacationHours(const int&);
    void setExtraHours(const int&); 
    void setBank(Bank*);

    void takeHoursOff(const int&);
    void requestExtraHours(const int&);
    Client& searchClient(const string& id); // should be implemented
    void EnableAccount(Account&) const; // should be implemented
    void DisableAccount(Account&) const; // should be implemented
    void CreateAccount(Client&) const; // should be implemented

    Employee& operator=(const Employee&);
    operator std::string() const;
protected:
    int _personnelId;
    int64_t _baseIncome;
    int64_t _penalty = 0;
    int64_t _reward = 0;
    int _vacationHours;
    int _extraHours;
    Bank* _bank;

    static const int maxLeaveHours = 15;
    static const int maxExtraHours = 12;
    static const int64_t leavePenalty = 100000;
    static const int64_t extraReward = 120000;

    Employee();
    void reset();
};

#endif // EMPLOYEE_H