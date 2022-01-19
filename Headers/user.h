#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
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

    string getUsername() const;
    string getPassword() const;

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

private:
    static vector<string> _usernames;
};

#endif // USER_H