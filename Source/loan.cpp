#include "../Headers/loan.h"
#include "../Headers/account.h"
#include "../Headers/bank.h"
#include <sstream>

vector<string> Loan::_loanSerials;

Loan::Loan(const string& serialNumber, const Account* account, const Date& loanDate, const int64_t& value,
    const LoanType& loanType) : Loan(serialNumber, account, loanDate, value, loanType, 0, 0) {}

Loan::Loan(const string& serialNumber, const Account* account, const Date& loanDate, const int64_t& value, const LoanType& loanType,
    const int& remaining, const int& overdue) : _loanDate{loanDate}, _loanType{loanType} {
        if(serialNumber.length() != 8) throw LoanException{"Loan Exception: Invalid serial number!"};
        _serialNumber = serialNumber;
        _linkedAccountNumber = account->getAccountNumber();
        setRemainingPayments(remaining);
        setValue(value);
        setOverduePayments(overdue);

        if(isValidSerial(serialNumber)) _loanSerials.push_back(serialNumber);
    }

Loan::Loan(const Loan& other) : _serialNumber{other._serialNumber}, _linkedAccountNumber{other._linkedAccountNumber}, _loanDate{other._loanDate},
    _value{other._value}, _loanType{other._loanType}, _remaining_payments{other._remaining_payments}, _overdue_payments{other._overdue_payments} {}

Loan::Loan(Loan&& other) noexcept : Loan{other} { other.reset(); }

Loan::~Loan() { reset(); }

void Loan::reset() {
    _serialNumber = "";
    _linkedAccountNumber = "";
    _loanDate = Date{1, 1, 1390};
    _value = 0;
    _remaining_payments = 0;
    _overdue_payments = 0;
    _loanType = LoanType::MONTH_12;
}

void Loan::setValue(const int64_t& value) {
    if(value < 0) throw LoanException{"Value cannot be less than 0!"};
    _value = value;
}

void Loan::setRemainingPayments(const int& remaining) {
    if(remaining < 0) throw LoanException{"Remaining payments cannot be less than 0!"};
    _remaining_payments = remaining;
}

void Loan::setOverduePayments(const int& overdue) {
    if(overdue < 0) throw LoanException{"Overdue payments cannot be less than 0!"};
    _overdue_payments = overdue;
}

string Loan::getSerialNumber(void) const { return _serialNumber; }
string Loan::getLinkedAccountNumber(void) const { return _linkedAccountNumber; }
Date Loan::getLoanDate(void) const { return _loanDate; }
int64_t Loan::getValue(void) const { return _value; }
LoanType Loan::getLoanType(void) const { return _loanType; }
int Loan::getRemainingPayments(void) const { return _remaining_payments; }
int Loan::getPaidPayments(void) const { 
    switch(_loanType) {
        case LoanType::MONTH_12: return (12 - _remaining_payments); 
        case LoanType::MONTH_24: return (24 - _remaining_payments); 
        case LoanType::MONTH_36: return (36 - _remaining_payments); 
    }
    return 0;
}
int Loan::getOverduePayments(void) const { return _overdue_payments; }
int64_t Loan::getEachPayment(void) const { 
    switch(_loanType) {
        case LoanType::MONTH_12: return (_value + ((_value * (int)_loanType) / 100)) / 12;
        case LoanType::MONTH_24: return (_value + ((_value * (int)_loanType) / 100)) / 24;
        case LoanType::MONTH_36: return (_value + ((_value * (int)_loanType) / 100)) / 36;
    }
    return 0;
}

bool Loan::isValidSerial(const string& serial) {
    for(string str : _loanSerials) {
        if(serial == str) return false;
    }
    return true;
}

void Loan::pay(Bank* bank) {
    if(_remaining_payments == 0 && _overdue_payments == 0) return;
    Account* acc = bank->searchAccount(_linkedAccountNumber);
    if(acc == nullptr) throw LoanException{"Account does not exist in the specified bank!"};
    if(acc->getBalance() >= getEachPayment()) {
        acc->setBalance(acc->getBalance() - getEachPayment());
        if(_overdue_payments > 0) _overdue_payments--;
        else _remaining_payments--;
    }
    else {
        if(_remaining_payments != 0) {
            _overdue_payments++;
            _remaining_payments--;
        }
    }
}

Loan& Loan::operator=(const Loan& rhs) {
    if(&rhs == this) return *this;
    _serialNumber = rhs._serialNumber;
    _linkedAccountNumber = rhs._linkedAccountNumber;
    _loanDate = rhs._loanDate;
    _value = rhs._value;
    _remaining_payments = rhs._remaining_payments;
    _overdue_payments = rhs._overdue_payments;
    _loanType = rhs._loanType;
    return *this;
}

Loan::operator std::string() const {
    stringstream str;
    str << "Serial number: " << _serialNumber << endl;
    str << "Linked Account number: " << _linkedAccountNumber << endl;
    str << "Loan date: " << _loanDate << endl;
    str << "Value: " << _value << " partition: " << getEachPayment() << endl;

    switch(_loanType) {
        case LoanType::MONTH_12:
            str << "Loan Type: 12-month payment with " << (int)_loanType << "% interest" << endl;
            break;
        case LoanType::MONTH_24:
            str << "Loan Type: 24-month payment with " << (int)_loanType << "% interest" << endl;
            break;
        case LoanType::MONTH_36:
            str << "Loan Type: 36-month payment with " << (int)_loanType << "% interest" << endl;
            break;
    }
    
    str << "Status: number of " << getPaidPayments() << " were paid successfully and " << _remaining_payments << " is remaining "
        << " and number of " << _overdue_payments << " is overdone!" << endl;

    return str.str();
}