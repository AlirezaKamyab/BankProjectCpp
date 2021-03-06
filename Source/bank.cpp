#include "../Headers/bank.h"
#include "../Headers/client.h"
#include "../Headers/employee.h"
#include "../Headers/facilities.h"
#include "../Headers/manager.h"
#include "../Headers/loan.h"
#include "../Headers/helperClass.h"
#include <fstream>
#include <sstream>

Bank::Bank() : _manager{nullptr}, _facilities{nullptr} {}
Bank::Bank(Manager* manager, const string& name, const int& branch) : Bank{manager, nullptr, name, branch} {}
Bank::Bank(Manager* manager, Facilities* facilities, const string& name, const int& branch) : _manager{manager},
        _facilities{facilities}, _name{name}, _branch{branch} {
    _employees.push_back(manager);
    if(facilities != nullptr) _employees.push_back(facilities);
    _start = Helper::getCurrentDate();
    _end = Helper::getCurrentDate();
}
Bank::Bank(const Bank& other) : Bank{other._manager, other._facilities, other._name, other._branch} {
    for(Employee* emp : other._employees) _employees.push_back(emp); 
    for(Client* cli : other._clients) _clients.push_back(cli);
    _start = other._start;
    _end = other._end;
}
Bank::Bank(Bank&& other) noexcept : Bank{other._manager, other._facilities, other._name, other._branch} {
    _clients = std::move(other._clients);
    _employees = std::move(other._employees);
    _start = std::move(other._start);
    _end = std::move(other._end);
    other.reset();
}
Bank::~Bank() {
    reset();
}

void Bank::reset() {
    for(Client* client : _clients) delete client;
    for(Employee* emp : _employees) delete emp;

    _manager = nullptr;
    _facilities = nullptr;
    _clients.clear();
    _employees.clear();
    _start = Date{1,1,1};
    _end = Date{1,1,1};
    _name = "";
    _branch = 0;
}

void Bank::addEmployee(Employee* employee) {
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
    for(Client* client : _clients) {
        Loan* loan = client->searchLoan(serialNumber);
        if(loan != nullptr) return loan;
    }

    return nullptr;
}

Account* Bank::searchAccount(const string& accountNumber) const {
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

User* Bank::searchUsername(const string& username) const {
    for(Client* client : _clients) {
        if(client->getUsername() == username) return client;
    }

    for(Employee* emp : _employees) {
        if(emp->getUsername() == username) return emp;
    }

    return nullptr;
}

const Loan* Bank::searchLoan(const string& serial) const {
    for(Client* client : _clients) {
        const Loan* loan = client->searchLoan(serial);
        if(loan != nullptr) return loan;
    }

    return nullptr;
}

string Bank::withdrawLoan() {
    stringstream msg;
    for(Client* client : _clients) {
        for(Account* account : client->_accounts) {
            if(account->getLoan() == nullptr) continue;
            account->getLoan()->pay(this);

            // writing a report
            stringstream str;
            str << "Account with national code " << client->getId() << " pay loan debt with value " << account->getLoan()->getEachPayment()
            << " in date " << Helper::getCurrentDate() << " time " << Helper::getCurrentTime() << endl;
            str << endl;
            writeToReport(REPORT_FILE_NAME, str.str());
            //------------------

            if(account->getLoan()->getRemainingPayments() == 0) {
                delete account->getLoan();
                account->setValidationCount(0);
                account->setLoan(nullptr);
                for(Account* a : client->_accounts) a->changeStatus(true);
                continue;
            }

            if(account->getLoan()->getOverduePayments() == 1) {
                msg << "Overdue payment for account number >> " << account->getAccountNumber() << endl;
            }
            if(account->getLoan()->getOverduePayments() >= 2 || account->getStatus() == false) {
                bool flag = true;
                for(Account* acc : client->_accounts) {
                    if(acc->getLoan() == nullptr) continue;
                    if(acc->getBalance() < acc->getLoan()->getOverduePayments() * acc->getLoan()->getEachPayment()) {
                        flag = false;
                        break;
                    }
                }
                if(flag) {
                    for(Account* a : client->_accounts) a->changeStatus(true);
                }
                else {
                    for(Account* a : client->_accounts) a->changeStatus(false);
                    msg << "All of the accounts of client with ID " << client->getId() << " is deactivated!" << endl;

                    // writing a report
                    stringstream str;
                    str << "Account with national code " << client->getId() << " banned" << " in date " << Helper::getCurrentDate() << " time " << Helper::getCurrentTime() << endl;
                    str << endl;
                    writeToReport(REPORT_FILE_NAME, str.str());
                    //------------------
                }
            }
        }
    }

    return msg.str();
}

const Client* Bank::searchClient(const string& id) const { return _searchClient(id); }
const Employee* Bank::searchEmployee(const int& personnelId) const { return _searchEmployee(personnelId); }
const Loan* Bank::searchLoans(const string& serialNumber) const { return _searchLoans(serialNumber); }
const Client* Bank::ownerOfTheAccount(const string& accountNumber) const { return _ownerOfTheAccount(accountNumber); }
Date Bank::getStartDate() const { return _start; }
Date Bank::getEndDate() const { return _end; }
const Manager* Bank::getManager() const { return _manager; }
const Facilities* Bank::getFacility() const { return _facilities; }
Employee* Bank::logAsEmployee(const string& username, const string& password) const {
    for(Employee* employee : _employees) {
        if(employee->login(User{username, password})) return employee;
    }

    return nullptr;
}

const Employee* Bank::searchEmployee(const string& id) const {
    for(Employee* emp : _employees) {
        if(emp->getId() == id) return emp;
    }

    return nullptr;
}

Client* Bank::logAsClient(const string& username, const string& password) const {
    for(Client* client : _clients) {
        if(client->login(username, password)) return client;
    }
    return nullptr;
}

void Bank::readClientFromFile(const string& filename) {
    ifstream file;
    file.open(filename);

    if(!file.is_open()) throw BankException{"Could not open the file!"};

    string name, lastname, id, username, password;
    Date bdate;
    while(getline(file, name)) {
        getline(file, lastname);
        getline(file, id);
        
        string temp;
        getline(file, temp);
        bdate = Date::str_to_date(temp);

        getline(file, username);
        getline(file, password);

        Client* client = new Client{name, lastname, id, bdate, username, password};
        _clients.push_back(client);
    }

    file.close();
}

void Bank::readAccountFromFile(const string& filename) {
    ifstream file;
    file.open(filename);
    if(!file.is_open()) throw BankException{"Could not open the file!"};

    string accountNumber, ownerId, strCreationDate;
    Date creationDate;
    int64_t balance;
    int validationCount, status;

    while(getline(file, accountNumber)) {
        if(accountNumber.length() == 0) continue;
        getline(file, ownerId);

        getline(file, strCreationDate);
        creationDate = Date::str_to_date(strCreationDate);

        file >> balance;
        file >> validationCount;
        file >> status;

        for(Client* client : _clients) {
            if(client->getId() == ownerId) {
                Account* account = new Account{accountNumber, ownerId, creationDate, balance, validationCount, (bool) status, this};
                client->_accounts.push_back(account);
                break;
            }
        }
    }

    file.close();
}

void Bank::readLoanFromFile(const string& filename) {
    ifstream file;
    file.open(filename);
    if(!file.is_open()) throw BankException{"Could not open the file!"};

    string serial, accountNumber, strCreationDate;
    Date creationDate;
    int64_t value;
    LoanType loanType;
    int remaining;
    int overdue;

    while(getline(file, serial)) {
        if(serial.length() != 8) continue;
        getline(file, accountNumber);

        getline(file, strCreationDate);
        creationDate = Date::str_to_date(strCreationDate);

        file >> value;
        
        int temp;
        file >> temp;
        loanType = (LoanType) temp;

        file >> remaining;
        file >> overdue;

        for(Client* client : _clients) {
            Account* account = client->getAccount(accountNumber);
            if(account == nullptr) continue;
            
            Loan* loan = new Loan{serial, account, creationDate, value, loanType, remaining, overdue};
            account->setLoan(loan);
            break;
        }
    }

    file.close();
}

void Bank::readRequestFromFile(const string& filename) {
    ifstream file;
    file.open(filename);

    if(!file.is_open()) throw BankException{"Could not open the file!"};

    string clientId, accountNumber;
    LoanType loanType;

    while(getline(file, clientId)) {
        if(clientId.length() == 0) continue;
        getline(file, accountNumber);

        int temp;
        file >> temp;
        loanType = (LoanType) temp;

        Client* client = _searchClient(clientId);
        Account* account = nullptr;
        if(client != nullptr) {
            account = client->getAccount(accountNumber);
        }

        if(account != nullptr) Facilities::addLoanRequest(Request{client, account, loanType});
    }

    file.close();
}

void Bank::readEmployeeFromFile(const string& filename) {
    ifstream file;
    file.open(filename);

    if(!file.is_open()) throw BankException{"Could not open the file!"};

    string name, lastname, id, username, password, strBirthday;
    int personnelId;
    Date birthday;
    int64_t baseIncome, reward, penalty;
    int leaveHours, extraHours;
    EmployeeType empType;

    while(getline(file, name)) {
        if(name.length() == 0) continue;
        getline(file, lastname);
        getline(file, id);
        
        string temp;
        getline(file, temp);
        birthday = Date::str_to_date(temp);

        getline(file, username);
        getline(file, password);

        file >> personnelId;
        file >> baseIncome;
        file >> leaveHours;
        file >> extraHours;
        file >> penalty;
        file >> reward;
        
        int numberType;
        file >> numberType;
        empType = (EmployeeType) numberType;

        if(_facilities != nullptr && empType == EmployeeType::FACILITIES) continue;
        if(empType == EmployeeType::MANAGER) {
            _manager->setPenalty(penalty);
            _manager->setExtraHours(extraHours);
            _manager->setReward(reward);
            _manager->setVacationHours(leaveHours);
            continue;
        }

        Employee* emp = new Employee{name, lastname, id, personnelId, birthday, username, password, baseIncome, leaveHours, extraHours, this};
        emp->setEmployeeType(empType);
        emp->setPenalty(penalty);
        emp->setReward(reward);

        if(emp->getEmployeeType() == EmployeeType::FACILITIES) {
            Facilities* ftemp = (Facilities*)emp;
            _facilities = ftemp;
        }

        if(personnelId >= Employee::getLastgGeneratedPersonnelId()) Employee::setLastGeneratedPersonnelId(personnelId);

        _employees.push_back(emp);
    }

    file.close();
}

void Bank::readDateFromFile(const string& filename) {
    ifstream file;
    file.open(filename, ios::binary);

    if(!file.is_open()) throw BankException{"Could not open the file!"};

    file.read((char*) &_start, sizeof(Date));
    file.read((char*) &_end, sizeof(Date));

    file.close();
}

void Bank::writeClientToFile(const string& filename) const {
    ofstream file;
    file.open(filename);

    if(!file.is_open()) throw BankException{"Could not open the file!"};

    for(Client* client : _clients) {
        file << client->getName() << endl;
        file << client->getLastName() << endl;
        file << client->getId() << endl;
        file << client->getBirthday() << endl;
        file << client->getUsername() << endl;
        file << client->getPassword() << endl;
    }

    file.close();
}

void Bank::writeAccountToFile(const string& filename) const {
    ofstream file;
    file.open(filename);

    if(!file.is_open()) throw BankException{"Could not open the file!"};

    for(Client* client : _clients) {
        for(Account* account : client->_accounts) {
            file << account->getAccountNumber() << endl;
            file << account->getOwnersId() << endl;
            file << account->getCreationDate() << endl;
            file << account->getBalance() << endl;
            file << account->getValidationCount() << endl;
            file << account->getStatus() << endl;
        }
    }

    file.close();
}
void Bank::writeLoanToFile(const string& filename) const {
    ofstream file;
    file.open(filename);

    if(!file.is_open()) throw BankException{"Could not open the file!"};
    
    for(Client* client : _clients) {
        for(Account* account : client->_accounts) {
            Loan* loan = account->getLoan();
            if(loan == nullptr) continue;
            file << loan->getSerialNumber() << endl;
            file << loan->getLinkedAccountNumber() << endl; 
            file << loan->getLoanDate() << endl;
            file << loan->getValue() << endl;
            file << (int) loan->getLoanType() << endl;
            file << loan->getRemainingPayments() << endl;
            file << loan->getOverduePayments() << endl;
        }
    }

    file.close();
}
void Bank::writeRequestToFile(const string& filename) const {
    ofstream file;
    file.open(filename);

    if(!file.is_open()) throw BankException{"Could not open the file!"};
    
    for(Request req : Facilities::requests) {
        file << req.getClient()->getId() << endl;
        file << req.getAccount()->getAccountNumber() << endl;
        file << (int) req.getType() << endl;
    }

    file.close();
}
void Bank::writeEmployeeToFile(const string& filename) const {
    ofstream file;
    file.open(filename);

    if(!file.is_open()) throw BankException{"Could not open the file!"};
    
    for(Employee* emp : _employees) {
        file << emp->getName() << endl;
        file << emp->getLastName() << endl;
        file << emp->getId() << endl;
        file << emp->getBirthday() << endl;
        file << emp->getUsername() << endl;
        file << emp->getPassword() << endl;
        file << emp->getPersonnelId() << endl;
        file << emp->getBaseIncome() << endl;
        file << emp->getVacationHours() << endl;
        file << emp->getExtraHours() << endl;
        file << emp->getPenalty() << endl;
        file << emp->getReward() << endl;
        file << (int) emp->getEmployeeType() << endl;
    }

    file.close();
}

void Bank::writeDateToFile(const string& filename) const {
    ofstream file;
    file.open(filename, ios::binary);

    if(!file.is_open()) throw BankException{"Could not open the file!"};

    file.write((char*) &_start, sizeof(Date));
    file.write((char*) &_end, sizeof(Date));

    file.close();
}

string Bank::endOfTheDay() {
    stringstream str;
    str << withdrawLoan();

    for(Client* client : _clients) {
        for(Account* account : client->_accounts) {
            if(account->getBalance() >= 1e6) {
                account->setValidationCount(account->getValidationCount() + 1);
            }
        }
    }

    if(_end.get_day() == Date::max_day_for_month(_end.get_month(), _end.get_year())) {
        _end.inc_one_day();

        for(Employee* emp : _employees) {
            emp->setVacationHours(0);
            emp->setExtraHours(0);
            emp->setPenalty(0);
            emp->setReward(0);
        }
    }
    else _end.inc_one_day();

    return str.str();
}

void Bank::writeToReport(const string& filename, const string& msg) const {
    ofstream file;
    file.open(filename, ios::app);

    if(!file.is_open()) throw BankException{"Cannot open the file!"};

    file << msg << endl;

    file.close();
}

Bank& Bank::operator=(const Bank& rhs) {
    if(&rhs == this) return *this;
    _manager = rhs._manager;
    _facilities = rhs._facilities;
    _clients = rhs._clients;
    _employees = rhs._employees;
    return *this;
}