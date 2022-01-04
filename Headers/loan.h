#ifndef LOAN_H
#define LOAN_H

#include <string>
#include "date.h"
using namespace std;

class Account;

class LoanException : public exception {
public:
    LoanException() : message{""} {}
    explicit LoanException(const string& msg) : message{msg} {}
    explicit LoanException(const char* msg) : message{msg} {}
    virtual ~LoanException() {}

    virtual const char* what() const noexcept { return message.c_str(); }
private:
    string message;
};

enum class LoanType {MONTH_12 = 4, MONTH_24 = 8, MONTH_36 = 12};

class Loan {
public:
    Loan(const string&, const Account*, const Date&, const int64_t&, const LoanType& = LoanType::MONTH_24);
    Loan(const string&, const Account*, const Date&, const int64_t&, const LoanType&, const int&, const int&);
    Loan(const Loan&);
    Loan(Loan&&) noexcept;
    ~Loan();

    void setValue(const int64_t&);
    void setRemainingPayments(const int&);
    void setOverduePayments(const int&);

    string getSerialNumber(void) const;
    string getLinkedAccountNumber(void) const;
    Date getLoanDate(void) const;
    int64_t getValue(void) const;
    LoanType getLoanType(void) const;
    int getRemainingPayments(void) const;
    int getPaidPayments(void) const;
    int getOverduePayments(void) const;
    int64_t getEachPayment(void) const;

    Loan& operator=(const Loan&);
    operator string() const;
private:
    string _serialNumber;
    string _linkedAccountNumber;
    Date _loanDate;
    int64_t _value;
    LoanType _loanType;
    int _remaining_payments;
    int _overdue_payments;

    Loan();
    void reset();
};

#endif // LOAN_H