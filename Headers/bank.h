#ifndef BANK_H
#define BANK_H

#include <vector>
#include <string>
using namespace std;

const string REPORT_FILE_NAME = "Report.txt";

class User;
class Client;
class Employee;
class Facilities;
class Manager;
class Loan;
class Account;

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
    const Loan* searchLoan(const string&) const;
    const Client* ownerOfTheAccount(const string&) const;
    Account* searchAccount(const string&) const;
    Employee* logAsEmployee(const string&, const string&) const;
    Client* logAsClient(const string&, const string&) const;
    User* searchUsername(const string&) const;
    string withdrawLoan();
    string endOfTheDay();

    void readClientFromFile(const string&);
    void readAccountFromFile(const string&);
    void readLoanFromFile(const string&);
    void readRequestFromFile(const string&);
    void readEmployeeFromFile(const string&);
    
    void writeClientToFile(const string&);
    void writeAccountToFile(const string&);
    void writeLoanToFile(const string&);
    void writeRequestToFile(const string&);
    void writeEmployeeToFile(const string&);

    void writeToReport(const string&, const string&);

    Bank& operator=(const Bank& other);
    friend class Employee;
    friend class Facilities;
    friend class Manager;
    friend class Account;
private:
    vector<Client*> _clients;
    vector<Employee*> _employees;
    Manager* _manager;
    Facilities* _facilities;
    
    Bank();
    void reset();
    void addEmployee(Employee*);
    void addClient(Client*);
    Client* _searchClient(const string&) const;
    Employee* _searchEmployee(const int&) const;
    Loan* _searchLoans(const string&) const;
    Client* _ownerOfTheAccount(const string&) const;
};

#endif // BANK_H