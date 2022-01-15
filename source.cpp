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
Client* makeClient(const string&);

void clearConsole();
void awaitKey();

int main() {
    cout << Helper::getCurrentDate() << endl;
    cout << (string)Helper::getCurrentTime() << endl;
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
    cout << "4- Loop up client" << endl;
    cout << "5- Create account" << endl;
    cout << "6- Remove account" << endl;
    cout << "7- Log out" << endl;
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
        else if(input == 3) {
            isRunning = false;
        }
    }
}

void staffMenu(Bank* bank) {
    string username;
    string password;

    cout << "Username >> ";
    getline(cin, username);
    if(cin.peek() == '\n') getchar();

    cout << "Password >> ";
    getline(cin, password);
    if(cin.peek() == '\n') getchar();

    Employee* logged = bank->logAsEmployee(username, password);
    cout << "Specified credentials are not correct!" << endl;
    awaitKey();

    if(logged == nullptr) return;

    bool isRunning = true;
    while(isRunning) {
        clearConsole();
        printStaffMenu(logged);
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
            clearConsole();
            bool searchRunning = true;
            while(searchRunning) {
                try {
                    string id;
                    cout << "Input ID or 'Q' to abort >> ";
                    cin >> id;

                    if(id == "Q") return;

                    cout << (string)*logged->searchClient(id) << endl;
                    cout << endl;
                }
                catch(exception& ex) {
                    cout << ex.what() << endl;
                }
                awaitKey();
            }
        }
        else if(input == '5') {
            clearConsole();
            Client* client = nullptr;

            try {
                string id;
                cout << "ID >> ";
                cin >> id;
                if(cin.peek() == '\n') getchar();

                client = logged->searchClient(id);

                int64_t balance;
                cout << "Deposite >> ";
                cin >> balance;

                if(client != nullptr) logged->createAccount(client, balance);
                else {
                    client = makeClient(id);
                    logged->createAccount(client, balance);
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
        else if(input == '7') {
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

        cout << "Name >> ";
        getline(cin, name);
        if(cin.peek() == '\n') getchar();

        cout << "Lastname >> ";
        getline(cin, lastname);
        if(cin.peek() == '\n') getchar();

        id = Id;

        cout << "Username >> ";
        getline(cin, username);
        if(cin.peek() == '\n') getchar();

        cout << "Password >> ";
        getline(cin, password);
        if(cin.peek() == '\n') getchar();

        cout << "Birthday >> ";
        cin >> bdate;
        cin.ignore();

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