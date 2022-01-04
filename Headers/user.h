#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

class UserException : public exception {
public:
    UserException() : _message{""} {}
    explicit UserException(const string& msg) : _message{msg} {}
    explicit UserException(const char* msg) : _message{msg} {}
    virtual ~UserException() {}

    virtual const char* what() const noexcept { return _message.c_str(); }

private:
    string _message;
};

class User {
public:
    User(const string&, const string&);
    User(const User&);
    User(User&&) noexcept;
    virtual ~User();

    bool login(const User&) const;
    bool login(const string&, const string&) const;

    User& operator=(const User&);
    bool operator==(const User&) const;
    bool operator!=(const User&) const;
    operator string() const;
protected:
    string _username;
    string _password;

    User();
    void reset();
};

#endif // USER_H