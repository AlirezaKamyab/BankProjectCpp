#ifndef FACILITIES_H
#define FACILITIES_H

#include <vector>
#include "employee.h"
#include "request.h"
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

    static void addLoanRequest(const Request&);
    string acceptARequest();
    void disableAccounts(Client*) const;
    void disableAccounts(const string&) const;
    string showAllrequests() const;
    string loanInfo(const string&) const;

    Facilities& operator=(const Facilities&);
    friend class Bank;
private:
    void reset();
    Facilities();
    static vector<Request> requests;
    static bool _acceptedOneRequest;
};

#endif // FACILITIES_H