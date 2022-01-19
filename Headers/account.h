#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <vector>
#include "date.h"
using namespace std;

class Loan;

class AccountException : public exception {
public:
    AccountException() : message{""} {}
    explicit AccountException(const string& msg) : message{msg} {}
    explicit AccountException(const char* msg) : message{msg} {}
    virtual ~AccountException() {}

    virtual const char* what() const noexcept { return message.c_str(); }
private:
    string message;
};

class Bank;

class Account {
public:
    Account(const string&, const string&, const Date&, Bank*);
    Account(const string&, const string&, const Date&, const int64_t&, Bank*);
    Account(const string&, const string&, const Date&, const int64_t&, const int&, const bool&, Bank*);
    Account(const Account&);
    Account(Account&&) noexcept;
    ~Account();

    void changeStatus(const bool&);
    void setBalance(const int64_t&);
    void setValidationCount(const int&);
    void setAccountNumber(const string&);

    string getAccountNumber() const;
    string getOwnersId() const;
    bool getStatus() const;
    int64_t getBalance() const;
    int getValidationCount() const;
    Bank* getBank() const;
    Loan* getLoan() const;
    void setLoan(Loan*);
    Date getCreationDate() const;

    Account& operator=(const Account&);
    operator string() const;

private:
    string _accountNumber;
    string _ownersId;
    Date _creationDate;
    int64_t _balance;
    int _validationCount;
    bool _activated;
    Bank* _bank;
    Loan* _loan;

    const static int64_t validAmount = 1e6;

    Account();
    void reset();
};

#endif // ACCOUNT_H