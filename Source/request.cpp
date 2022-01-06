#include "../Headers/request.h"
#include <sstream>

Request::Request() : Request{nullptr, nullptr, LoanType::MONTH_24} {}
Request::Request(Client* client, Account* account, const LoanType& loanType) : _client{client}, _account{account}, _type{loanType} {}
Request::Request(const Request& other) : Request{other._client, other._account, other._type} {}
Request::Request(Request&& other) noexcept : Request{other} { other.reset(); }
Request::~Request() {}

void Request::reset() {
    _client = nullptr;
    _account = nullptr;
    _type = LoanType::MONTH_12;
}

LoanType Request::getType() const { return _type; }
Client* Request::getClient() const { return _client; }
Account* Request::getAccount() const { return _account; }

Request& Request::operator=(const Request& rhs) {
    if(&rhs == this) return *this;
    _client = rhs._client;
    _account = rhs._account;
    _type = rhs._type;
    return *this;
}

Request::operator std::string() const {
    stringstream str;
    str << _account->operator std::string() << endl;
    
    switch (_type)
    {
    case LoanType::MONTH_12:
        str << "12_Month, " << (int)_type << "%" << endl;
        break;
    case LoanType::MONTH_24:
        str << "24_Month, " << (int)_type << "%" << endl;
        break;
    case LoanType::MONTH_36:
        str << "36_Month, " << (int)_type << "%" << endl;
        break;
    }

    return str.str();
}