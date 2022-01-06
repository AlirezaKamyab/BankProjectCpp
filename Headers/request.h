#ifndef REQUEST_H
#define REQUEST_H

#include "loan.h"
#include "client.h"
#include "account.h"
#include <string>
using namespace std;

class Request{
public:
    Request(Client*, Account*, const LoanType& = LoanType::MONTH_24);
    Request(const Request&);
    Request(Request&&) noexcept;
    ~Request();

    LoanType getType() const;
    Client* getClient() const;
    Account* getAccount() const;

    Request& operator=(const Request&);
    operator std::string() const;
private:
    LoanType _type;
    Client* _client;
    Account* _account;
    
    Request();
    void reset();
};

#endif // REQUEST_H