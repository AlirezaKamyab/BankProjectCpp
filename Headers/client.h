#ifndef CLIENT_H
#define CLIENT_H

#include "user.h"
#include "person.h"
#include "loan.h"
#include <vector>
using namespace std;

class Account;
class Facilities;
class Bank;

class ClientException : public exception {
public:
    ClientException() : message{""} {}
    explicit ClientException(const string& msg) : message{msg} {}
    explicit ClientException(const char* msg) : message{msg} {}
    virtual ~ClientException() {}

    virtual const char* what(void) const noexcept { return message.c_str(); }
private:
    string message;
};

class Client : public Person, public User {
public:
    Client(const string&, const string&, const string&, const Date&, const string&, const string&);
    Client(const string&, const string&, const string&, const Date&, const string&, const string&, const vector<Account*>&);
    Client(const Client&);
    Client(Client&&) noexcept;
    ~Client();

    Bank* getBank(void) const;
    void setBank(Bank* bank);

    void createAccount(Account&);
    Account* getAccount(const string&) const;
    void deposit(const string&, const int64_t&) const;
    void withdraw(const string&, const int64_t&) const;
    void requestLoan(const string&, const LoanType&);
    string showLoans() const;

    Client& operator=(const Client&);
    operator string() const;

    friend class Employee;
    friend class Facilities;
    friend class Bank;
private:
    vector<Account*> _accounts;
    Bank* _bank;
    
    Client();
    void reset();
};

#endif // CLIENT_H