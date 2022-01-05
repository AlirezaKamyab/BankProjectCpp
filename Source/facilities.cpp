#include "../Headers/facilities.h"
#include "../Headers/bank.h"
#include "../Headers/account.h"
#include "../Headers/request.h"
#include <sstream>
#include <iomanip>

vector<Request> Facilities::requests{};
int Facilities::lastSerialGenerated = 0;

Facilities::Facilities() : Employee{"", "", "", 0, Date{1, 1, 1390}, "", "", 0, 0, 0, nullptr} {}

Facilities::Facilities(const string& name, const string& lastname, const string& id, const int& personnelId, const Date& birthday,
    const string& username, const string& password) : Facilities{name, lastname, id, personnelId, birthday, username, password,
    0, 0, 0, nullptr} {}

Facilities::Facilities(const string& name, const string& lastname, const string& id, const int& personnelId, const Date& birthday,
    const string& username, const string& password, const int64_t& baseIncome, const int& vacationHours, const int& extraHours,
    Bank* bank) : Employee{name, lastname, id, personnelId, birthday, username, password, baseIncome, vacationHours, extraHours,
    bank}, _acceptedARequest{false} {}

Facilities::Facilities(const Facilities& other) : Employee{other} {}
Facilities::Facilities(Facilities&& other) noexcept : Facilities{other} { other.reset(); }
Facilities::~Facilities() { reset(); }

void Facilities::reset() {
    Employee::reset();
}

void Facilities::addLoanRequest(const Request& request) {
    requests.push_back(request);
}

void Facilities::acceptARequest() {
    for(int i = 0; i < requests.size(); i++) {
        Account* temp = requests[i].getAccount();
        if(temp->getStatus() == false) continue;
        if(temp->getValidationCount() == 0 || temp->getBalance() < 1e6) continue;
        int64_t newAmount = temp->getValidationCount() * temp->getBalance();
        stringstream serial;
        serial << setw(8) << lastSerialGenerated++;
        Date loanCreationDate{1,1,1390}; // this should be changed
        Loan* loan = new Loan{serial.str(), temp, loanCreationDate, newAmount, requests[i].getType()};
        requests.erase(requests.begin() + i);
        return;
    }
}

void Facilities::disableAccounts(Client* client) const {
    for(Account* account : client->_accounts) account->changeStatus(false);
}

string Facilities::loanInfo(const string& serialNumber) const {
    const Loan* loan = _bank->searchLoans(serialNumber);
    return loan->operator std::string();
}

Facilities& Facilities::operator=(const Facilities& rhs) {
    if(&rhs == this) return *this;
    Employee::operator=(rhs);
    return *this;
}