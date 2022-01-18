#include <iostream>
#include "Headers/manager.h"
#include "Headers/employee.h"
#include "Headers/client.h"
#include "Headers/account.h"
#include "Headers/facilities.h"
#include "Headers/bank.h"
#include "Headers/helperClass.h"
using namespace std;

void printMainMenu(Employee*);
void printStaffMenu();
void mainMenu(Bank*);
void staffMenu(Bank*);
bool awaitConfirmation(const char, const string&);
Client* makeClient(const string&);

void clearConsole();
void awaitKey();

int main() {
    Manager* manager = new Manager{"Alireza", "Kamyab", "1850484211", Employee::generatePersonnelId(), Date{27,4,1381}, "aimlessly", "123456789"};
    manager->setBaseIncome(1000000000);
    Bank* aimlessly = new Bank{manager};
    manager->setBank(aimlessly);

    Employee* emp = new Employee{"Mahdi", "Yousefi", "123456789", Employee::generatePersonnelId(), Date{1,1,1399}, "aaa", "aaaaaaaaa", 3500000, 0, 0, aimlessly};
    manager->hireEmployee(emp);
    mainMenu(aimlessly);
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
        printMainMenu();
        char input;
        cin >> input;

        if(input == '1') {
            staffMenu(bank);
        }
        else if(input == '2') {

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

    Facilities* flogged = nullptr;
    Manager* mlogged = nullptr;

    if(logged->getEmployeeType() == EmployeeType::FACILITIES) {
        flogged = dynamic_cast<Facilities*>(logged);
    }
    else if(logged->getEmployeeType() == EmployeeType::MANAGER) {
        mlogged = dynamic_cast<Manager*>(logged);
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
                cout << "Done!" << endl;
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
                        logged->createAccount(client, balance);
                    }
                }
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
            try {
                string id;
                cout << "Id >>";
                cin >> id;

                flogged->disableAccounts(id);
            }
            catch(exception& ex) {
                cout << ex.what() << endl;
            }
            awaitKey();
        }
        else if(input == 'b' && flogged != nullptr) {
            try {
                cout << flogged->showAllrequests() << endl;
            }
            catch(exception& ex) {
                cout << ex.what() << endl;
            }
            awaitKey();
        }
        else if(input == 'c' && flogged != nullptr) {
            try {
                string res = flogged->acceptARequest();
                if(res == "") cout << "No request has been accepted!" << endl;
                else {
                    cout << "    A request has been accepted" << endl;
                    cout << "-----------------------------------" << endl;
                    cout << flogged->loanInfo(res) << endl;
                    cout << endl;
                }
            }
            catch(exception& ex) {
                cout << ex.what() << endl;
            }
            awaitKey();
        }
        else if(input == 'd' && flogged != nullptr) {
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

                Employee* emp = new Employee{name, lastname, id, Employee::generatePersonnelId(), bdate, username, password, baseIncome,0, 0, bank};
                mlogged->hireEmployee(emp);

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