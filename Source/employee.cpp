#include "../Headers/employee.h"
#include <sstream>

Employee::Employee() : Person{"", "", "", Date{1,1,1390}}, User{"Anonymouse", "Anonymouse"}, 
    _vacationHours{0}, _extraHours{0}, _baseIncome{0}, _personnelId{0} {}

Employee::Employee(const string& name, const string& lastname, const string& id, const int& personnelId, const Date& bdate,
    const string& username, const string& password) : Employee{name, lastname, id, personnelId, bdate, username, password,
    0, 0, 0, nullptr} {}

Employee::Employee(const string& name, const string& lastname, const string& id, const int& personnelId, const Date& bdate,
    const string& username, const string& password, const int64_t& baseIncome, const int& vacationHours, const int& extraHours,
    Bank* bank) : Person{name, lastname, id, bdate}, User{username, password}, _baseIncome{baseIncome}, _extraHours{extraHours},
    _vacationHours{vacationHours}, _personnelId{personnelId}, _bank{bank} {
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

void Employee::setBank(Bank* bank) {
    if(bank == nullptr) throw EmployeeException{"Bank cannot be set to null after the initialization!"};
    _bank = bank;
}

void Employee::takeHoursOff(const int& hours) {
    if(_penalty >= getIncome()) throw EmployeeException{"Too much leave hours has been recorded!"};

    _vacationHours += hours;
    if(_vacationHours >= Employee::maxLeaveHours) _penalty = (_vacationHours - Employee::maxLeaveHours) * Employee::leavePenalty;
}

void Employee::requestExtraHours(const int& hours) {
    if(hours + _extraHours > Employee::maxExtraHours) throw EmployeeException{"Amount of working extra hours have been exceeded the maximum!"};

    _extraHours += hours;
    _reward = Employee::extraReward * _extraHours;
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
    str << "Personnel ID: " << _personnelId << endl;
    str << "Base Income: " << _baseIncome << endl;
    str << "Income: " << getIncome() << endl;
    str << "Leave Hours: " << _vacationHours << " with penalty: " << _penalty << endl;
    str << "Extra working hours: " << _extraHours << " with reward: " << _reward << endl;
    str << User::operator std::string() << endl;
    return str.str();
}