#ifndef CLIENT_H
#define CLIENT_H

#include "user.h"
#include "person.h"
#include "loan.h"
#include <vector>
using namespace std;

class Account;
class Facilities;
class Client : public Person, public User {
public:
    Client(const string&, const string&, const string&, const Date&, const string&, const string&);
    Client(const string&, const string&, const string&, const Date&, const string&, const string&, const vector<Account*>&);
    Client(const Client&);
    Client(Client&&) noexcept;
    ~Client();

    void createAccount(Account&);
    Account* getAccount(const string&) const;
    void deposit(const string&, const int64_t&) const;
    void withdraw(const string&, const int64_t&) const;
    void requestLoan(const string&, const LoanType&); // should be implemented

    Client& operator=(const Client&);
    operator string() const;
    friend class Facilities;
private:
    vector<Account*> _accounts;
    
    Client();
    void reset();
};

#endif // CLIENT_H