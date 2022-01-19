#include "../Headers/client.h"
#include "../Headers/account.h"
#include "../Headers/bank.h"
#include "../Headers/facilities.h"
#include "../Headers/request.h"
#include <sstream>

Client::Client() : Person{"", "", "", Date{}}, User{"Anonymouse", "Anonymouse"} {}

Client::Client(const string& name, const string& lastname, const string& id, const Date& bdate,
        const string& username, const string& password) : Person{name, lastname, id, bdate}, User{username, password} {}

Client::Client(const string& name, const string& lastname, const string& id, const Date& bdate,
        const string& username, const string& password, const vector<Account*>& accounts) : 
        Person{name, lastname, id, bdate}, User{username, password}, _accounts{accounts} {}

Client::Client(const Client& other) : Person{other._name, other._lastname, other._id, other._birthday},
        User{other._username, other._password}, _accounts{other._accounts} {}

Client::Client(Client&& other) noexcept : Client{other} {
    other.reset();
}

Client::~Client() { reset(); }

void Client::reset() {
    User::reset();
    Person::reset();
    for(Account* account : _accounts) delete account;
    _accounts = vector<Account*>();
}

void Client::createAccount(Account& account) {
    _accounts.push_back(&account);
}

Account* Client::getAccount(const string& accountNumber) const {
    for(Account* account : _accounts) {
        if(account->getAccountNumber() == accountNumber) return account;
    }

    return nullptr;
}

void Client::deposit(const string& accountNumber, const int64_t& amount) const {
    if(amount < 0) throw AccountException{"Cannot deposit a negative amount"};
    Account* temp = getAccount(accountNumber);
    if(temp == nullptr) throw AccountException{"Account with the specified account number does not exists!"};
    temp->setBalance(temp->getBalance() + amount);
}

void Client::withdraw(const string& accountNumber, const int64_t& amount) const {
    if(amount < 0) throw AccountException{"Cannot withdraw a negative amount"};
    Account* temp = getAccount(accountNumber);
    if(temp == nullptr) throw AccountException{"Account with the specified account number does not exists!"};
    if(temp->getStatus() == false) throw AccountException{"Account is de-activated!"};
    if(temp->getBalance() < amount) throw AccountException{"Insufficient balance!"};
    temp->setBalance(temp->getBalance() - amount);
}

void Client::requestLoan(const string& accountNumber, const LoanType& type) {
    Account* temp = getAccount(accountNumber);
    if(temp == nullptr) throw AccountException{"Account with the specified account number does not exists!"};
    if(temp->getBalance() < 1e6) throw AccountException{"Account balance should be at least 1,000,000"};
    if(temp->getStatus() == false) throw AccountException{"Account is de-activated!"};

    Request req{this, temp, type};

    Facilities::addLoanRequest(req);
}

string Client::showLoans() const {
    stringstream str;

    for(Account* account : _accounts) {
        if(account->getLoan() == nullptr) continue;
        str << (string) *account->getLoan() << "\n" << endl;
    }

    return str.str();
}

Loan* Client::searchLoan(const string& serial) const {
    for(Account* account : _accounts) {
        if(account->getLoan() == nullptr) continue;
        if(account->getLoan()->getSerialNumber() == serial) return account->getLoan();
    }

    return nullptr;
}

Client& Client::operator=(const Client& rhs) {
    if(&rhs == this) return *this;
    User::operator=(rhs);
    Person::operator=(rhs);
    _accounts = rhs._accounts;
    return *this;
}

Client::operator std::string() const {
    stringstream str;
    str << Person::operator std::string() << endl;
    str << User::operator std::string() << endl;

    str << "         Accounts" << endl;
    str << "---------------------------" << endl;
    for(Account* account : _accounts) str << account->operator std::string() << endl;

    return str.str();
}