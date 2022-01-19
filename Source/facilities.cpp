#include "../Headers/facilities.h"
#include "../Headers/bank.h"
#include "../Headers/account.h"
#include "../Headers/request.h"
#include "../Headers/helperClass.h"
#include "../Headers/helperClass.h"
#include <sstream>
#include <iomanip>

vector<Request> Facilities::requests{};
bool Facilities::_acceptedOneRequest = false;

Facilities::Facilities() : Employee{"", "", "", 0, Date{1, 1, 1390}, "", "", 0, 0, 0, nullptr} {}

Facilities::Facilities(const string& name, const string& lastname, const string& id, const int& personnelId, const Date& birthday,
    const string& username, const string& password) : Facilities{name, lastname, id, personnelId, birthday, username, password,
    0, 0, 0, nullptr} {}

Facilities::Facilities(const string& name, const string& lastname, const string& id, const int& personnelId, const Date& birthday,
    const string& username, const string& password, const int64_t& baseIncome, const int& vacationHours, const int& extraHours,
    Bank* bank) : Employee{name, lastname, id, personnelId, birthday, username, password, baseIncome, vacationHours, extraHours,
    bank} {
        _employeeType = EmployeeType::FACILITIES;
    }

Facilities::Facilities(const Facilities& other) : Employee{other} {}
Facilities::Facilities(Facilities&& other) noexcept : Facilities{other} { other.reset(); }
Facilities::~Facilities() { reset(); }

void Facilities::reset() {
    Employee::reset();
}

void Facilities::addLoanRequest(const Request& request) {
    for(Request req : requests) {
        if(req.getAccount()->getAccountNumber() == request.getAccount()->getAccountNumber()) throw FacilitiesException{"Previous request is still pending!"};
    }
    requests.push_back(request);
}

string Facilities::acceptARequest() {
    if(_acceptedOneRequest) throw FacilitiesException{"One request has been accepted for today"};
    if(requests.size() == 0) return "";
    Account* temp = requests[0].getAccount();
    if(temp->getLoan() != nullptr) {
        requests.erase(requests.begin() + 0);
        return acceptARequest();
    }
    if(temp->getStatus() == false) {
        requests.erase(requests.begin() + 0);
        return acceptARequest();
    }
    if(temp->getValidationCount() == 0 || temp->getBalance() < 1e6) {
        requests.erase(requests.begin() + 0);
        return acceptARequest();
    }

    int64_t newAmount = temp->getValidationCount() * temp->getBalance();
    Date loanCreationDate = Helper::getCurrentDate();
    
    string serial;
    while(true) {
        serial = Helper::generateRandom(8);
        if(Loan::isValidSerial(serial)) break;
    }

    int remaining = 0;
    switch(requests[0].getType()) {
        case LoanType::MONTH_12:
            remaining = 12;
            break;
        case LoanType::MONTH_24:
            remaining = 24;
            break;
        case LoanType::MONTH_36:
            remaining = 36;
            break;
    }

    Loan* loan = new Loan{serial, temp, loanCreationDate, newAmount, requests[0].getType(), remaining, 0};
    temp->setBalance(temp->getBalance() + newAmount);
    requests.erase(requests.begin() + 0);
    temp->setLoan(loan);
    return serial;
}

void Facilities::disableAccounts(Client* client) const {
    for(Account* account : client->_accounts) account->changeStatus(false);
}

void Facilities::disableAccounts(const string& id) const {
    Client* client = searchClient(id);
    if(client == nullptr) throw BankException{"Couldn't find the specified client inside the bank!"};
    disableAccounts(client);
}

string Facilities::showAllrequests() const {
    stringstream str;
    for(Request req : requests) {
        str << req.operator std::string() << "\n" << endl;
    }
    return str.str();
}

string Facilities::loanInfo(const string& serialNumber) const {
    const Loan* loan = _bank->searchLoans(serialNumber);
    if(loan == nullptr) throw LoanException{"No loan found with specified serial number"};
    return loan->operator std::string();
}

Facilities& Facilities::operator=(const Facilities& rhs) {
    if(&rhs == this) return *this;
    Employee::operator=(rhs);
    return *this;
}