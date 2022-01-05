#include "../Headers/request.h"

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