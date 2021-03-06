#include "../Headers/manager.h"
#include "../Headers/bank.h"
#include <sstream>

Manager::Manager() : Employee{"", "", "", 0, Date{1, 1, 1390}, "", "", 0, 0, 0, nullptr} {}

Manager::Manager(const string& name, const string& lastname, const string& id, const int& personnelId, const Date& birthday,
    const string& username, const string& password) : Manager{name, lastname, id, personnelId, birthday, username, password,
    0, 0, 0, nullptr} {}

Manager::Manager(const string& name, const string& lastname, const string& id, const int& personnelId, const Date& birthday,
    const string& username, const string& password, const int64_t& baseIncome, const int& vacationHours, const int& extraHours, Bank* bank) :
    Employee{name, lastname, id, personnelId, birthday, username, password, baseIncome, vacationHours, extraHours, bank} {
        _employeeType = EmployeeType::MANAGER;
    }

Manager::Manager(const Manager& other) : Employee{other} {}
Manager::Manager(Manager&& other) noexcept : Manager{other} { other.reset(); }
Manager::~Manager() { reset(); }

void Manager::reset() {
    Employee::reset();
}

string Manager::employeeInfo(const int& personnelId) const {
    const Employee* emp = _bank->searchEmployee(personnelId);
    if(emp == nullptr) throw BankException{"Could't find the specified employee inside the bank!"};
    return (string) *emp;
}

void Manager::hireEmployee(Employee* emp) const {
    // Bank handles the duplicate employees being added
    if(_bank->searchUsername(emp->getUsername()) != nullptr) throw BankException{"Username already exists!"};
    _bank->addEmployee(emp);
    if(emp->getEmployeeType() == EmployeeType::FACILITIES) _bank->_facilities = dynamic_cast<Facilities*>(emp);
}

void Manager::fireEmployee(const int& personnelId) const {
    for(int i = 0; i < _bank->_employees.size(); i++) {
        if(_bank->_employees[i]->getPersonnelId() == personnelId) {
            if(_bank->_employees[i]->getId() == getId()) throw ManagerException{"Manager cannot fire him/her self!"};
            delete _bank->_employees[i];
            _bank->_employees.erase(_bank->_employees.begin() + i);
            return;
        }
    }

    throw ManagerException{"No such employee with specified personnel id found inside bank."};
}

Manager& Manager::operator=(const Manager& rhs) {
    if(&rhs == this) return *this;
    Employee::operator=(rhs);
    return *this;
}
