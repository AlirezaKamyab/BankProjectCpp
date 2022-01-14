#include "../Headers/user.h"
#include <sstream>

User::User() {}
User::User(const string& username, const string& password) {
    if(username.length() < 3) throw UserException{"Invalid username! username should be at least 3 characters."};
    if(password.length() < 8) throw UserException{"Invalid password! password should be at least 8 characters."};

    _username = username;
    _password = password;

    for(string user : _usernames) {
        if(user == username) throw UserException{"Username is taken try another one!"};
    }

    _usernames.push_back(username);
}

User::User(const User& other) {
    _username = other._username;
    _password = other._password;
}
User::User(User&& other) noexcept : User(other) { other.reset(); }
User::~User() { reset(); }

bool User::login(const User& user) const {
    return *this == user;
}

bool User::login(const string& username, const string& password) const {
    return (_username == username && _password == password);
}

void User::reset() {
    _username = "";
    _password = "";
}

User& User::operator=(const User& rhs) {
    if(&rhs == this) return *this;
    _username = rhs._username;
    _password = rhs._password;
    return *this;
}

bool User::operator==(const User& rhs) const {
    return (_username == rhs._username && _password == rhs._password);
}

bool User::operator!=(const User& rhs) const {
    return !(*this == rhs);
}

User::operator std::string() const {
    stringstream str;
    str << "Username: " << _username << endl;
    str << "Password: " << _password << endl;
    return str.str();
}
