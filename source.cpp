#include <iostream>
#include <sstream>
#include "Headers/manager.h"
#include "Headers/employee.h"
#include "Headers/client.h"
#include "Headers/account.h"
#include "Headers/facilities.h"
#include "Headers/bank.h"
#include "Headers/helperClass.h"
using namespace std;

void reportClientLogin(Bank*, const Person*);
void reportStaffLogin(Bank*, const Person*);
void reportAccountCreation(Bank*, const Person*);
void reportDeposite(Bank*, const Person*, const int64_t&);
void reportLoan(Bank*, const Person*, const int64_t&);
void reportLeave(Bank*, const Person*, const int&);
void reportBan(Bank*, const Person*);

void printMainMenu();
void printStaffMenu(Employee*);
void printClientMenu(Client*);
void mainMenu(Bank*);
void staffMenu(Bank*);
void clientMenu(Bank*);
bool awaitConfirmation(const char, const string&);
Client* makeClient(const string&);

void clearConsole();
void awaitKey();

int main() {
    Manager* manager = new Manager{"Alireza", "Kamyab", "1850484211", Employee::generatePersonnelId(), Date{27,4,1381}, "aimlessly", "123456789"};
    manager->setBaseIncome(20000000);
    Bank* aimlessly = new Bank{manager, "Central Bank Of America", 165};
    manager->setBank(aimlessly);

    try { aimlessly->readDateFromFile("date.information"); } catch(exception& ex) {}
    try { aimlessly->readClientFromFile("clients.txt"); } catch(exception& ex) {}
    try { aimlessly->readAccountFromFile("accounts.txt"); } catch(exception& ex) {}
    try { aimlessly->readLoanFromFile("loans.txt"); } catch(exception& ex) {}
    try { aimlessly->readRequestFromFile("requests.txt"); } catch(exception& ex) {}
    try { aimlessly->readEmployeeFromFile("employees.txt"); } catch(exception& ex) {}

    mainMenu(aimlessly);
    cout << aimlessly->endOfTheDay();

    aimlessly->writeDateToFile("date.information");
    aimlessly->writeClientToFile("clients.txt");
    aimlessly->writeAccountToFile("accounts.txt");
    aimlessly->writeLoanToFile("loans.txt");
    aimlessly->writeRequestToFile("requests.txt");
    aimlessly->writeEmployeeToFile("employees.txt");

    delete aimlessly;
    awaitKey();
    return 0;
}

void printMainMenu() {
    cout << "           Main Menu" << endl;
    cout << "--------------------------------" << endl;
    cout << "1- Staff" << endl;
    cout << "2- Client" << endl;
    cout << "3- Exit" << endl;
    cout << ">> ";
}

void printStaffMenu(Employee* emp) {
    cout << emp->getName() << " " << emp->getLastName() << endl;
    cout << "             Staff Menu" << endl;
    cout << "--------------------------------------" << endl;
    cout << "1- Request leave time" << endl;
    cout << "2- Request extra working time" << endl;
    cout << "3- Personal Information" << endl;
    cout << "4- Look up client" << endl;
    cout << "5- Create account" << endl;
    cout << "6- Remove account" << endl;
    if(emp->getEmployeeType() == EmployeeType::FACILITIES) {
        cout << "a- Disable all accounts" << endl;
        cout << "b- Show all requests" << endl;
        cout << "c- Check a request for accept" << endl;
        cout << "d- Check loan status" << endl;
    }
    else if(emp->getEmployeeType() == EmployeeType::MANAGER) {
        cout << "a- Employee information" << endl;
        cout << "b- Hire employee" << endl;
        cout << "c- Fire employee" << endl;
    }
    cout << "0- Log out" << endl;
    cout << ">> ";
}

void mainMenu(Bank* bank) {
    bool isRunning = true;
    while(isRunning) {
        clearConsole();
        // cout << Date::max_day_for_month(bank->getEndDate().get_month(), bank->getEndDate().get_year()) - bank->getEndDate().get_day() << endl;
        printMainMenu();
        char input;
        cin >> input;

        if(input == '1') {
            staffMenu(bank);
        }
        else if(input == '2') {
            clientMenu(bank);
        }
        else if(input == '3') {
            isRunning = false;
        }
    }
}

void staffMenu(Bank* bank) {
    clearConsole();
    string username;
    string password;

    cin.ignore();

    cout << "Username >> ";
    getline(cin, username);

    cout << "Password >> ";
    getline(cin, password);

    Employee* logged = nullptr;

    try {
        logged = bank->logAsEmployee(username, password);
    }
    catch(exception& ex) {}

    if(logged == nullptr) {
        cout << "Specified credentials are not correct!" << endl;
        awaitKey();
        return;
    }

    reportStaffLogin(bank, logged);

    Facilities* flogged = nullptr;
    Manager* mlogged = nullptr;

    if(logged->getEmployeeType() == EmployeeType::FACILITIES) {
        flogged = (Facilities*) logged;
    }
    else if(logged->getEmployeeType() == EmployeeType::MANAGER) {
        mlogged = (Manager*) logged;
    }

    bool isRunning = true;
    while(isRunning) {
        clearConsole();
        
        if(flogged != nullptr) printStaffMenu(flogged);
        else if(mlogged != nullptr) printStaffMenu(mlogged);
        else printStaffMenu(logged);

        char input;
        cin >> input;

        if(input == '1') {
            clearConsole();
            try {
                int time;
                cout << "Input duration >> ";
                cin >> time;
                logged->takeHoursOff(time);
                cout << "Remaining hours " << max(0, 15 - logged->getVacationHours()) << endl;
                cout << "Penalty is now " << logged->getPenalty() << " new income is " << logged->getIncome() << endl;
                reportLeave(bank, logged, time);
            }
            catch(exception& ex) {
                cout << ex.what() << endl;
                cout << "Aborting!" << endl;
            }
            awaitKey();
        }
        else if(input == '2') {
            clearConsole();
            try {
                int time;
                cout << "Input duration >> ";
                cin >> time;
                logged->requestExtraHours(time);
                cout << "Reward is " << logged->getReward() << " new income is " << logged->getIncome() << endl;
            }
            catch(exception& ex) {
                cout << ex.what() << endl;
            }
            awaitKey();
        }
        else if(input == '3') {
            clearConsole();
            try {
                cout << (string)*logged << endl;
            }
            catch(exception& ex) {
                cout << ex.what() << endl;
            }
            awaitKey();
        }
        else if(input == '4') {
            bool searchRunning = true;
            while(searchRunning) {
                clearConsole();
                try {
                    string id;
                    cout << "Input ID or 'Q' to abort >> ";
                    cin >> id;

                    if(id == "Q") {
                        break;
                    }
                    Client* searched = logged->searchClient(id);
                    if(searched == nullptr) {
                        cout << "No client with specified id found!" << endl;
                        awaitKey();
                        continue;
                    }
                    cout << (string) *searched << endl;
                    cout << endl;
                    cout << "         Loans" << endl;
                    cout << "------------------------" << endl;
                    cout << searched->showLoans() << endl;
                    cout << endl;

                    awaitKey();
                }
                catch(exception& ex) {
                    cout << ex.what() << endl;
                }
            }
        }
        else if(input == '5') {
            clearConsole();
            Client* client = nullptr;
            bool flag = false;

            try {
                string id;
                cout << "ID >> ";
                cin >> id;
                if(cin.peek() == '\n') getchar();

                client = logged->searchClient(id);

                int64_t balance = 0;

                while(true) {
                    cout << "Deposite >> ";
                    cin >> balance;

                    if(balance >= 50000) break;
                    else {
                        flag = awaitConfirmation('Q', "Deposite should be at least 50,000, Input Q to abort or anything otherwise!");
                        if(flag == true) break;
                    }
                }

                if(flag != true) {
                    if(client != nullptr) logged->createAccount(client, balance);
                    else {
                        client = makeClient(id);
                        if(client != nullptr) logged->createAccount(client, balance);
                    }
                }

                if(client != nullptr) reportAccountCreation(bank, client);
            }
            catch (exception& ex) {
                if(client != nullptr) delete client;
                cout << ex.what() << endl;
            }
            awaitKey();
        }
        else if(input == '6') {
            clearConsole();

            try {
                string accountNumber;
                cout << "Account number >> ";
                cin >> accountNumber;

                logged->deleteAccount(accountNumber);
                cout << "Done" << endl;
            }
            catch(exception& ex) {
                cout << ex.what() << endl;
            }
            awaitKey();
        }
        else if(input == 'a' && flogged != nullptr) {
            clearConsole();
            try {
                string id;
                cout << "Id >>";
                cin >> id;

                flogged->disableAccounts(id);

                reportBan(bank, bank->searchClient(id));
            }
            catch(exception& ex) {
                cout << ex.what() << endl;
            }
            awaitKey();
        }
        else if(input == 'b' && flogged != nullptr) {
            clearConsole();
            try {
                cout << flogged->showAllrequests() << endl;
            }
            catch(exception& ex) {
                cout << ex.what() << endl;
            }
            awaitKey();
        }
        else if(input == 'c' && flogged != nullptr) {
            clearConsole();
            try {
                string serial = flogged->acceptARequest();
                if(serial == "") cout << "No request has been accepted!" << endl;
                else {
                    cout << "    A request has been accepted" << endl;
                    cout << "-----------------------------------" << endl;
                    cout << flogged->loanInfo(serial) << endl;
                    cout << endl;

                    const Loan* loan = bank->searchLoan(serial); 
                    reportLoan(bank, bank->ownerOfTheAccount(loan->getLinkedAccountNumber()), loan->getValue());
                }
            }
            catch(exception& ex) {
                cout << ex.what() << endl;
            }
            awaitKey();
        }
        else if(input == 'd' && flogged != nullptr) {
            clearConsole();
            try {
                string serial;
                cout << "Serial >> ";
                cin >> serial;

                cout << flogged->loanInfo(serial) << endl;
                cout << endl;
            }
            catch(exception& ex) {
                cout << ex.what() << endl;
            }
            awaitKey();
        }
        else if(input == 'a' && mlogged != nullptr) {
            clearConsole();
            try{
                int personnelId;
                cout << "Personnel ID >> ";
                cin >> personnelId;

                cout << mlogged->employeeInfo(personnelId) << endl;
                cout << endl;
            }
            catch(exception& ex) {
                cout << ex.what() << endl;
            }
            awaitKey();
        }
        else if(input == 'b' && mlogged != nullptr) {
            clearConsole();
            try {
                string name, lastname, username, password, id;
                Date bdate;
                int64_t baseIncome;

                cin.ignore();

                cout << "Name >> ";
                getline(cin, name);

                cout << "Lastname >> ";
                getline(cin, lastname);

                cout << "ID >> ";
                cin >> id;
                if(cin.peek() == '\n') getchar();

                cout << "Birthday >> ";
                cin >> bdate;
                if(cin.peek() == '\n') getchar();

                cout << "Base Income >> ";
                cin >> baseIncome;
                if(cin.peek() == '\n') getchar();

                cout << "Username >> ";
                getline(cin, username);

                cout << "Password >> ";
                getline(cin, password);

                EmployeeType employeeType = EmployeeType::EMPLOYEE;

                if(bank->getFacility() == nullptr) {
                    cout << "      Employee Type" << endl;
                    cout << "--------------------------" << endl;
                    cout << "1- Simple Employee" << endl;
                    cout << "2- Facilities Employee" << endl;

                    while(true) {
                        cout << ">> ";
                        char input;
                        cin >> input;

                        if(input == '1') {
                            employeeType = EmployeeType::EMPLOYEE;
                            break;
                        }
                        else if(input == '2') {
                            employeeType = EmployeeType::FACILITIES;
                            break;
                        }
                    }
                }
                
                if(employeeType == EmployeeType::EMPLOYEE) {
                    Employee* emp = new Employee{name, lastname, id, Employee::generatePersonnelId(), bdate, username, password, baseIncome,0, 0, bank};
                    mlogged->hireEmployee(emp);
                }
                else if(employeeType == EmployeeType::FACILITIES) {
                    Facilities* emp = new Facilities{name, lastname, id, Employee::generatePersonnelId(), bdate, username, password, baseIncome,0, 0, bank};
                    mlogged->hireEmployee(emp);
                }

                cout << "Done!" << endl;
            }
            catch(exception& ex) {
                cout << ex.what() << endl;
            }
            awaitKey();
        }
        else if(input == 'c' && mlogged != nullptr) {
            try {
                int personnelId;
                cout << "Personnel ID >> ";
                cin >> personnelId;
                
                mlogged->fireEmployee(personnelId);

                cout << "Done!" << endl;
            }
            catch(exception& ex) {
                cout << ex.what() << endl;
            }
            awaitKey();
        }
        else if(input == '0') {
            isRunning = false;
        }
        else; // Do nothing when input is invalid
    }
}

void printClientMenu(Client* client) {
    cout << client->getName() << " " << client->getLastName() << endl;
    cout << "         Client Menu" << endl;
    cout << "-------------------------------" << endl;
    cout << "1- Deposite" << endl;
    cout << "2- Withdraw" << endl;
    cout << "3- Request a loan" << endl;
    cout << "4- Show personal information" << endl;
    cout << "0- Log out" << endl;
    cout << ">> ";
}
void clientMenu(Bank* bank) {
    clearConsole();
    Client* client = nullptr;
    try {
        string username;
        string password;

        cin.ignore();

        cout << "Username >> ";
        getline(cin, username);
        
        cout << "Password >> ";
        getline(cin, password);

        client = bank->logAsClient(username, password);
    }
    catch(exception& ex) { client = nullptr; }

    if(client == nullptr) {
        cout << "Invalid credentials!" << endl;
        awaitKey();
        return;
    }

    reportClientLogin(bank, client);

    bool isRunning = true;
    while(isRunning) {
        clearConsole();
        printClientMenu(client);
        char input;
        cin >> input;

        if(input == '1') {
            clearConsole();
            try {
                int64_t amount;
                string accountNumber;

                cin.ignore();

                cout << "Account Number >> ";
                getline(cin, accountNumber);

                cout << "Amount >> ";
                cin >> amount;

                client->deposit(accountNumber, amount);
                reportDeposite(bank, client, amount);

                cout << "Done!" << endl;
            }
            catch(exception& ex) {
                cout << ex.what() << endl;
            }
            awaitKey();
        }
        else if(input == '2') {
            clearConsole();
            try {
                int64_t amount;
                string accountNumber;

                cin.ignore();

                cout << "Account Number >> ";
                getline(cin, accountNumber);

                cout << "Amount >> ";
                cin >> amount;

                client->withdraw(accountNumber, amount);

                cout << "Done!" << endl;
            }
            catch(exception& ex) {
                cout << ex.what() << endl;
            }
            awaitKey();
        }
        else if(input == '3') {
            clearConsole();
            try {
                cin.ignore();
                string accountNumber;
                cout << "Account Number >> ";
                getline(cin, accountNumber);

                LoanType loanType;
                cout << "        Loan Type" << endl;
                cout << "--------------------------" << endl;
                cout << "1- 12 Month with 4% interest" << endl;
                cout << "2- 24 Month with 8% interest" << endl;
                cout << "3- 36 Month with 12% interest" << endl;

                while(true) {
                    cout << ">> ";
                    char input;
                    cin >> input;

                    if(input == '1') {
                        loanType = LoanType::MONTH_12;
                        cout << "12 Month with 4% interest selected" << endl;
                        break;
                    }
                    else if(input == '2') {
                        loanType = LoanType::MONTH_24;
                        cout << "24 Month with 8% interest selected" << endl;
                        break;
                    }
                    else if(input == '3') {
                        loanType = LoanType::MONTH_36;
                        cout << "36 Month with 12% interest selected" << endl;
                        break;
                    }
                }

                client->requestLoan(accountNumber, loanType);
                cout << "Request has been sent!" << endl;
            }
            catch(exception& ex) {
                cout << ex.what() << endl;
            }
            awaitKey();
        }
        else if(input == '4') {
            try {
                bool infoRunning = true;
                while(infoRunning) {
                    clearConsole();
                    cout << (string) *client << endl << endl;
                    cout << "    Personal Information" << endl;
                    cout << "------------------------------" << endl;
                    cout << "1- Account info" << endl;
                    cout << "2- Loan info" << endl;
                    cout << "3- Back" << endl;
                    cout << ">> ";
                    char input;
                    cin >> input;

                    if(input == '1') {
                        try {
                            clearConsole();
                            cout << (string) *client << endl;
                            cout << "         Accounts" << endl;
                            cout << "---------------------------" << endl;
                            cout << client->showAccounts() << endl;
                        }
                        catch(exception& ex) {
                            cout << ex.what() << endl;
                        }
                        awaitKey();
                    }
                    else if(input == '2') {
                        try {
                            clearConsole();
                            cout << (string) *client << endl;
                            cout << "            Loans" << endl;
                            cout << "---------------------------------" << endl;
                            cout << client->showLoans() << endl;
                        }
                        catch(exception& ex) {
                            cout << ex.what() << endl;
                        }
                        awaitKey();
                    }
                    else if(input == '3') {
                        infoRunning = false;
                    }
                    else; // do nothing about invalid input
                }
            }
            catch(exception& ex) {
                cout << ex.what() << endl;
            }
            awaitKey();
        }
        else if(input == '0') {
            isRunning = false;
        }
        else; // do nothing about invalid input for the menu
    }
}

Client* makeClient(const string& Id) {
    Client* client = nullptr;
    try {
        string name, lastname, id, username, password;
        Date bdate;

        cin.ignore();

        cout << "Name >> ";
        getline(cin, name);

        cout << "Lastname >> ";
        getline(cin, lastname);

        id = Id;

        cout << "Username >> ";
        getline(cin, username);

        cout << "Password >> ";
        getline(cin, password);

        cout << "Birthday >> ";
        cin >> bdate;

        client = new Client{name, lastname, id, bdate, username, password};
        return client;
    }
    catch (exception& ex) {
        if(client == nullptr) delete client;
        cout << ex.what() << endl;
    }

    return nullptr;
}

void reportClientLogin(Bank* bank, const Person* person) {
    stringstream str;
    str << "Account with national code " << person->getId() << " login date " << Helper::getCurrentDate() << " time " << Helper::getCurrentTime() << endl;
    str << endl;
    bank->writeToReport(REPORT_FILE_NAME, str.str());
}

void reportStaffLogin(Bank* bank, const Person* person) {
    stringstream str;
    str << "Staff with national code " << person->getId() << " login date " << Helper::getCurrentDate() << " time " << Helper::getCurrentTime() << endl;
    str << endl;
    bank->writeToReport(REPORT_FILE_NAME, str.str());
}

void reportAccountCreation(Bank* bank, const Person* person) {
    stringstream str;
    str << "Account with national code " << person->getId() << " create in date " << Helper::getCurrentDate() << " time " << Helper::getCurrentTime() << endl;
    str << endl;
    bank->writeToReport(REPORT_FILE_NAME, str.str());
}

void reportDeposite(Bank* bank, const Person* person, const int64_t& amount) {
    stringstream str;
    str << "Account with national code " << person->getId() << " increase account balance with amount " << amount << 
    " in date " << Helper::getCurrentDate() << " time " << Helper::getCurrentTime() << endl;
    str << endl;
    bank->writeToReport(REPORT_FILE_NAME, str.str());
}

void reportLoan(Bank* bank, const Person* person, const int64_t& value) {
    stringstream str;
    str << "Account with national code " << person->getId() << " get loan with value " << value << 
    " in date " << Helper::getCurrentDate() << " time " << Helper::getCurrentTime() << endl;
    str << endl;
    bank->writeToReport(REPORT_FILE_NAME, str.str());
}

void reportLeave(Bank* bank, const Person* person, const int& time) {
    stringstream str;
    str << "Staff with national code " << person->getId() << " get " << time << " days rest" << 
    " in date " << Helper::getCurrentDate() << " time " << Helper::getCurrentTime() << endl;
    str << endl;
    bank->writeToReport(REPORT_FILE_NAME, str.str());
}

void reportBan(Bank* bank, const Person* person) {
    stringstream str;
    str << "Account with national code " << person->getId() << " banned in date " << Helper::getCurrentDate() << " time " << Helper::getCurrentTime() << endl;
    str << endl;
    bank->writeToReport(REPORT_FILE_NAME, str.str());
}

void clearConsole() {
    if(system("clear") == 0) return;
    system("cls");
}
void awaitKey() {
    system("pause");
}
bool awaitConfirmation(const char character, const string& msg) {
    cout << msg << endl;
    char input;
    cout << ">> ";
    cin >> input;

    if(input == character) return true;
    else return false;
}