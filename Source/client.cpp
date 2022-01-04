#include "../Headers/client.h"
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
    _accounts = vector<Account*>();
}

void Client::createAccount(Account& account) {
    _accounts.push_back(&account);
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

    // print accounts too?

    return str.str();
}