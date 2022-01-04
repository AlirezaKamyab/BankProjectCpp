#include "../Headers/facilities.h"

vector<Client*> Facilities::requests{};

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

void Facilities::addLoanRequest(Client* client) {
    for(Client* ptr : requests) {
        if(ptr == client) throw FacilitiesException{"Your previous request is still pending! request to remove it."};
    }

    requests.push_back(client);
}

void Facilities::removeLoanRequest(Client* client) {
    for(int i = 0; i < requests.size(); i++) {
        if(requests[i] == client) {
            requests.erase(requests.begin() + i);
            return;
        }
    }

    throw FacilitiesException{"No request found from the specified client!"};
}

Facilities& Facilities::operator=(const Facilities& rhs) {
    if(&rhs == this) return *this;
    Employee::operator=(rhs);
    return *this;
}