#ifndef BANK_H
#define BANK_H

#include <vector>
using namespace std;

class Client;
class Employee;
class Facilities;
class Manager;
class Loan;

class BankException : public exception {
public:
    BankException() : message{""} {}
    explicit BankException(const string& msg) : message{msg} {}
    explicit BankException(const char* msg) : message{msg} {}
    virtual ~BankException() {}

    virtual const char* what(void) const noexcept { return message.c_str(); } 
private:
    string message;
};

class Bank{
public:
    Bank(Manager*);
    Bank(Manager*, Facilities*);
    Bank(const Bank&);
    Bank(Bank&&) noexcept;
    ~Bank();

    const Client* searchClient(const string&) const;
    const Employee* searchEmployee(const int&) const;
    const Loan* searchLoans(const string&) const;
    const Manager* getManager() const;
    const Facilities* getFacility() const;

    Bank& operator=(const Bank& other);
    friend class Employee;
    friend class Facility;
    friend class Manager;
    friend class Account;
private:
    vector<Client*> _clients;
    vector<Employee*> _employees;
    vector<Loan*> _loans;
    Manager* _manager;
    Facilities* _facilities;
    
    Bank();
    void reset();
    void addEmployee(Employee*);
    void addClient(Client*);
    Client* _searchClient(const string&) const;
    Employee* _searchEmployee(const int&) const;
    Loan* _searchLoans(const string&) const;
};

#endif // BANK_H