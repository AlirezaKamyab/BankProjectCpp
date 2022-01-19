#include "../Headers/account.h"
#include "../Headers/loan.h"
#include <sstream>

Account::Account() : _accountNumber{""}, _ownersId{""}, _balance{0}, _activated{false}, _validationCount{0}, _creationDate{1,1,1390}, 
    _bank{nullptr}, _loan{nullptr} {}

Account::Account(const string& accountNumber, const string& ownersId, const Date& creationDate, Bank* bank) : Account{accountNumber,
    ownersId, creationDate, 0, 0, true, bank} {}

Account::Account(const string& accountNumber, const string& ownersId, const Date& creationDate, const int64_t& balance,
    const int& validationCount, const bool& status, Bank* bank) : _accountNumber{accountNumber}, _ownersId{ownersId}, _activated{status}, 
    _creationDate{creationDate}, _bank{bank}, _loan{nullptr} { 
        setBalance(balance);
        setValidationCount(validationCount);
    }

Account::Account(const Account& other) : Account{other._accountNumber, other._ownersId, other._creationDate, other._balance,
    other._validationCount, other._activated, other._bank} {
        _loan = other._loan;
    }

Account::Account(Account&& other) noexcept : Account{other} { other.reset(); }

Account::~Account() { reset(); }

void Account::reset() {
    _accountNumber = "";
    _ownersId = "";
    _balance = 0;
    _activated = false;
    _validationCount = 0;
    _bank = nullptr;
    delete _loan;
    _loan = nullptr;
    _creationDate = Date{1, 1, 1390};
}

void Account::setBalance(const int64_t& balance) {
    if(balance < 0) throw AccountException{"An account cannot have negative balance!"};
    _balance = balance;
}

void Account::setValidationCount(const int& validationCount) {
    if(validationCount < 0) throw AccountException{"Validation count cannot be less than zero!"};
    _validationCount = validationCount;
}

void Account::setAccountNumber(const string& accountNumber) {
    _accountNumber = accountNumber;
}

Loan* Account::getLoan() const { return _loan; }
void Account::setLoan(Loan* loan) { _loan = loan; } 

void Account::changeStatus(const bool& newStatus) { _activated = newStatus; }
string Account::getAccountNumber() const { return _accountNumber; }
string Account::getOwnersId() const { return _ownersId; }
int64_t Account::getBalance() const { return _balance; }
Date Account::getCreationDate() const { return _creationDate; }
int Account::getValidationCount() const { return _validationCount; }
bool Account::getStatus() const { return _activated; }
Bank* Account::getBank() const { return _bank; }

Account& Account::operator=(const Account& rhs) {
    if(&rhs == this) return *this;
    _accountNumber = rhs._accountNumber;
    _ownersId = rhs._ownersId;
    _balance = rhs._balance;
    _activated = rhs._activated;
    _validationCount = rhs._validationCount;
    _creationDate = rhs._creationDate;
    _bank = rhs._bank;
    _loan = rhs._loan;
    return *this;
}

Account::operator string() const {
    stringstream str;
    str << "Account number: " << _accountNumber << endl;
    str << "Owner's ID: " << _ownersId << endl;
    str << "Balance: " << _balance << endl;
    str << "Valid days: " << _validationCount << endl;
    str << "Status: " << ((_activated)? "Active" : "De-activated") << endl;
    str << "Creation date: " << _creationDate << endl;
    return str.str();
}