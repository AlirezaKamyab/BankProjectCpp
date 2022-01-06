#include "../Headers/bank.h"
#include "../Headers/client.h"
#include "../Headers/employee.h"
#include "../Headers/facilities.h"
#include "../Headers/manager.h"
#include "../Headers/loan.h"

Bank::Bank() : _manager{nullptr}, _facilities{nullptr} {}
Bank::Bank(Manager* manager) : Bank{manager, nullptr} {}
Bank::Bank(Manager* manager, Facilities* facilities) : _manager{manager}, _facilities{facilities} {}
Bank::Bank(const Bank& other) : Bank{other._manager, other._facilities} {
    for(Employee* emp : other._employees) _employees.push_back(emp); 
    for(Client* cli : other._clients) _clients.push_back(cli);
}
Bank::Bank(Bank&& other) noexcept : Bank{other._manager, other._facilities} {
    _clients = std::move(other._clients);
    _employees = std::move(other._employees);
    other.reset();
}
Bank::~Bank() {
    reset();
}

void Bank::reset() {
    _manager = nullptr;
    _facilities = nullptr;
    _clients.clear();
    _employees.clear();
}

void Bank::addEmployee(Employee* employee) {
    if(employee->getEmployeeType() != EmployeeType::EMPLOYEE) throw BankException{"A simple employee was needed, facility or manager was given!"};
    if(employee == nullptr) throw BankException{"Invalid employee!"};
    Employee* temp = _searchEmployee(employee->getPersonnelId());
    if(temp != nullptr) throw BankException{"Another employee with the same personnel id exists!"};
    _employees.push_back(employee);
}

void Bank::addClient(Client* client) {
    if(client == nullptr) throw BankException{"Invalid Client!"};
    Client* temp = _searchClient(client->getId());
    if(temp != nullptr) throw BankException{"Another client with the same id exists"};
    _clients.push_back(client);
}

Client* Bank::_searchClient(const string& id) const {
    for(Client* client : _clients) {
        if(client->getId() == id) return client;
    }
    return nullptr;
}

Employee* Bank::_searchEmployee(const int& personnelId) const {
    for(Employee* employee : _employees) {
        if(employee->getPersonnelId() == personnelId) return employee;
    }
    return nullptr;
}

Loan* Bank::_searchLoans(const string& serialNumber) const {
    for(Loan* loan : _loans) {
        if(loan->getSerialNumber() == serialNumber) return loan;
    }

    return nullptr;
}

Account* Bank::_searchAccount(const string& accountNumber) const {
    for(Client* client : _clients) {
        for(Account* account : client->_accounts) {
            if(account->getAccountNumber() == accountNumber) return account;
        }
    }
    return nullptr;
}

Client* Bank::_ownerOfTheAccount(const string& accountNumber) const {
    for(Client* client : _clients) {
        for(Account* account : client->_accounts) {
            if(account->getAccountNumber() == accountNumber) return client;
        }
    }
    return nullptr;
}

const Client* Bank::searchClient(const string& id) const { return _searchClient(id); }
const Employee* Bank::searchEmployee(const int& personnelId) const { return _searchEmployee(personnelId); }
const Loan* Bank::searchLoans(const string& serialNumber) const { return _searchLoans(serialNumber); }
const Manager* Bank::getManager() const { return _manager; }
const Facilities* Bank::getFacility() const { return _facilities; }
const vector<const Loan*> Bank::getClientLoans(const string& id) const {
    Client* client = _searchClient(id);
    if(client == nullptr) throw BankException{"Bank could not find the client!"};
    vector<const Loan*> temp;
    for(Loan* loan : _loans) {
        if(client->getAccount(loan->getLinkedAccountNumber()) != nullptr) temp.push_back(loan);
    }

    return temp;
}

Bank& Bank::operator=(const Bank& rhs) {
    if(&rhs == this) return *this;
    _manager = rhs._manager;
    _facilities = rhs._facilities;
    _clients = rhs._clients;
    _employees = rhs._employees;
    return *this;
}