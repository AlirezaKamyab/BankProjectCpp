#ifndef FACILITIES_H
#define FACILITIES_H

#include <vector>
#include "employee.h"
using namespace std;

class FacilitiesException : public EmployeeException {
public:
    FacilitiesException() : EmployeeException{""} {}
    explicit FacilitiesException(const string& msg) : EmployeeException{msg} {}
    explicit FacilitiesException(const char* msg) : EmployeeException{msg} {}
    virtual ~FacilitiesException() {}
};

class Facilities : public Employee {
public:
    Facilities(const string&, const string&, const string&, const int&, const Date&, const string&, const string&);
    Facilities(const string&, const string&, const string&, const int&, const Date&, const string&, const string&,
        const int64_t&, const int&, const int&, Bank*);
    Facilities(const Facilities&);
    Facilities(Facilities&&) noexcept;
    virtual ~Facilities();

    static void addLoanRequest(Client*);
    static void removeLoanRequest(Client*);
    string loanStatus() const; // should be implemented
    void acceptARequest(); // should be implemented
    void disableAccounts(Client*) const; // should be implemented
    string loanInfo(const string&) const; // should be implemented

    Facilities& operator=(const Facilities&);
private:
    static vector<Client*> requests;
    Facilities();
    void reset();
    bool _acceptedARequest;
};

#endif // FACILITIES_H