#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include "date.h"
using namespace std;

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

class Account {
public:
    Account(const string&, const string&, const Date&);
    Account(const string&, const string&, const Date&, const int64_t&);
    Account(const string&, const string&, const Date&, const int64_t&, const int&, const bool&);
    Account(const Account&);
    Account(Account&&) noexcept;
    ~Account();

    void changeStatus(const bool&);
    void setBalance(const int64_t&);
    void setValidationCount(const int&);

    string getAccountNumber() const;
    string getOwnersId() const;
    bool getStatus() const;
    int64_t getBalance() const;
    int getValidationCount() const;

    Account& operator=(const Account&);
    operator string() const;

private:
    string _accountNumber;
    string _ownersId;
    Date _creationDate;
    int64_t _balance;
    int _validationCount;
    bool _activated;

    const static int64_t validAmount = 1e6;
    static int constructedAccounts;

    Account();
    void reset();
};

#endif // ACCOUNT_H