#include "../Headers/employee.h"
#include "../Headers/bank.h"
#include "../Headers/account.h"
#include "../Headers/helperClass.h"
#include <sstream>

int Employee::lastGeneratedPersonnelId = 100;

Employee::Employee() : Person{"", "", "", Date{1,1,1390}}, User{"Anonymouse", "Anonymouse"}, 
    _vacationHours{0}, _extraHours{0}, _baseIncome{0}, _personnelId{0}, _employeeType{EmployeeType::EMPLOYEE} {}

Employee::Employee(const string& name, const string& lastname, const string& id, const int& personnelId, const Date& bdate,
    const string& username, const string& password) : Employee{name, lastname, id, personnelId, bdate, username, password,
    0, 0, 0, nullptr} {}

Employee::Employee(const string& name, const string& lastname, const string& id, const int& personnelId, const Date& bdate,
    const string& username, const string& password, const int64_t& baseIncome, const int& vacationHours, const int& extraHours,
    Bank* bank) : Person{name, lastname, id, bdate}, User{username, password}, _baseIncome{baseIncome}, _extraHours{extraHours},
    _vacationHours{vacationHours}, _personnelId{personnelId}, _bank{bank}, _employeeType{EmployeeType::EMPLOYEE} {
        if(_personnelId < 100 || _personnelId > 999) throw EmployeeException{"Invalid personnel id!"};
        if(_vacationHours < 0 || _extraHours < 0 || _baseIncome < 0) throw EmployeeException{"Some arguments were supposed to be non-negative; negative value was given!"};
    }

Employee::Employee(const Employee& other) : Employee{other._name, other._lastname, other._id, other._personnelId, other._birthday,
    other._username, other._password, other._baseIncome, other._vacationHours, other._extraHours, other._bank} {
        _penalty = other._penalty;
        _reward = other._reward;
    }

Employee::Employee(Employee&& other) noexcept : Employee{other} {
    other.reset();
}

Employee::~Employee() { reset(); }

void Employee::reset() {
    User::reset();
    Person::reset();
    _personnelId = 0;
    _vacationHours = 0;
    _extraHours = 0;
    _baseIncome = 0;
    _bank = nullptr;
}

int Employee::getPersonnelId() const { return _personnelId; }
Bank* Employee::getBank() const { return _bank; }
int64_t Employee::getBaseIncome() const { return _baseIncome; }
int64_t Employee::getIncome() const { return _baseIncome + _reward - _penalty; }
int64_t Employee::getReward() const { return _reward; }
int64_t Employee::getPenalty() const { return _penalty; }
int Employee::getExtraHours() const { return _extraHours; }
int Employee::getVacationHours() const { return _vacationHours; }
EmployeeType Employee::getEmployeeType() const { return _employeeType; }

void Employee::setBaseIncome(const int64_t& baseIncome) {
    if(baseIncome < 0) throw EmployeeException{"Invalid base income! Base income cannot be less than zero!"};
    _baseIncome = baseIncome;
}

void Employee::setPenalty(const int64_t& penalty) {
    if(penalty < 0) throw EmployeeException{"Invalid penalty! penalty cannot be less than zero!"};
    _penalty = penalty;
}

void Employee::setReward(const int64_t& reward) {
    if(reward < 0) throw EmployeeException{"Invalid reward! reward cannot be less than zero!"};
    _reward = reward;
}

void Employee::setExtraHours(const int& extraHours) {
    if(extraHours < 0) throw EmployeeException{"Invalid extra hours! extra hours should not be less than zero!"};
    _extraHours = extraHours;
}

void Employee::setVacationHours(const int& vacationHours) {
    if(vacationHours < 0) throw EmployeeException{"Invalid vacation hours! vacation hours cannot be less than zero!"};
    _vacationHours = vacationHours;
}

void Employee::setEmployeeType(const EmployeeType& type) { _employeeType = type; }

void Employee::setLastGeneratedPersonnelId(const int& lgpi) { lastGeneratedPersonnelId = lgpi; }
int Employee::getLastgGeneratedPersonnelId() { return lastGeneratedPersonnelId; }

void Employee::setBank(Bank* bank) {
    if(bank == nullptr) throw EmployeeException{"Bank cannot be set to null after the initialization!"};
    _bank = bank;
}

void Employee::takeHoursOff(const int& hours) {
    if(_penalty + (Employee::leavePenalty * (_vacationHours + hours - Employee::maxLeaveHours)) > getIncome()) throw EmployeeException{"Too much leave hours has been recorded!"};

    _vacationHours += hours;
    if(_vacationHours >= Employee::maxLeaveHours) _penalty = (_vacationHours - Employee::maxLeaveHours) * Employee::leavePenalty;
}

void Employee::requestExtraHours(const int& hours) {
    if(hours + _extraHours > Employee::maxExtraHours) throw EmployeeException{"Amount of working extra hours have been exceeded the maximum!"};

    _extraHours += hours;
    _reward = Employee::extraReward * _extraHours;
} 

Client* Employee::searchClient(const string& id) const {
    return _bank->_searchClient(id);
}

void Employee::enableAccount(const string& accountNumber) const {
    Account* account = _bank->searchAccount(accountNumber);
    if(account == nullptr) throw EmployeeException{"Cannot find the specified account in the bank!"};
    account->changeStatus(true);
}

void Employee::disableAccount(const string& accountNumber) const {
    Account* account = _bank->searchAccount(accountNumber);
    if(account == nullptr) throw EmployeeException{"Cannot find the specified account in the bank!"};
    account->changeStatus(false);
}

void Employee::createAccount(Client* client, const int64_t& startingBalance) const {
    if(_bank->searchEmployee(client->getId()) != nullptr) throw EmployeeException{"There is an employee with the same id!"};

    Client* su = nullptr;
    for(Client* c : _bank->_clients) {
        if(c->getUsername() == client->getUsername()) {
            su = c;
            break;
        }
    } 
    const Client* sc = _bank->searchClient(client->getId()); 
    if(su != nullptr && sc != nullptr && sc->getUsername() == su->getUsername() && sc->getId() != su->getId()) throw EmployeeException{"Username already exists!"};

    if(searchClient(client->getId()) == nullptr) _bank->addClient(client);
    Account* account = new Account{Helper::generateRandom(10), client->getId(), Helper::getCurrentDate(), startingBalance, 0, true, _bank};
    while(_bank->searchAccount(account->getAccountNumber()) != nullptr) account->setAccountNumber(Helper::generateRandom(10));
    client->createAccount(*account);
}

int Employee::generatePersonnelId() {
    return lastGeneratedPersonnelId++;
}

void Employee::deleteAccount(const string& accountNumber) const {
    Client* owner = _bank->_ownerOfTheAccount(accountNumber);
    if(owner == nullptr) throw EmployeeException{"Cound't find the specified account in the bank!"};
    for(int i = 0; i < owner->_accounts.size(); i++) {
        if(owner->_accounts[i]->getAccountNumber() == accountNumber) {
            if(owner->_accounts[i]->getLoan() != nullptr) throw BankException{"A loan found bound to this account!"};
            delete owner->_accounts[i];
            owner->_accounts.erase(owner->_accounts.begin() + i);
            break;
        }
    }


    if(owner->_accounts.size() == 0) {
        Person::deleteFromPersonIds(owner->getId());
        for(int i = 0; i < _bank->_clients.size(); i++) {
            if(_bank->_clients[i]->getId() == owner->getId()) {
                delete owner;
                _bank->_clients.erase(_bank->_clients.begin() + i);
                break;
            }
        }
    }

    return;
}

string Employee::showClientInfo(const string& id) const {
    Client* client = searchClient(id);
    if(client == nullptr) return "Not found!";
    return (string) *client;
}

Employee& Employee::operator=(const Employee& rhs) {
    if(&rhs == this) return *this;
    User::operator=(rhs);
    Person::operator=(rhs);
    _personnelId = rhs._personnelId;
    _vacationHours = rhs._vacationHours;
    _extraHours = rhs._extraHours;
    _baseIncome = rhs._baseIncome;
    _bank = rhs._bank;
    return *this;
}

Employee::operator std::string() const {
    stringstream str;
    str << Person::operator std::string() << endl;

    switch (_employeeType)
    {
    case EmployeeType::EMPLOYEE:
        str << "Type: Employee" << endl;
        break;
    case EmployeeType::FACILITIES:
        str << "Type: Facilities" << endl;
        break;
    case EmployeeType::MANAGER:
        str << "Type: Manager" << endl;
        break;
    default:
        str << "Type: Unknown" << endl;
        break;
    }

    str << "Personnel ID: " << _personnelId << endl;
    str << "Base Income: " << _baseIncome << endl;
    str << "Income: " << getIncome() << endl;
    str << "Leave Hours: " << _vacationHours << " with penalty: " << _penalty << endl;
    str << "Extra working hours: " << _extraHours << " with reward: " << _reward << endl;
    str << User::operator std::string() << endl;
    return str.str();
}