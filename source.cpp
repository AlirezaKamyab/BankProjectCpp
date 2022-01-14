#include <iostream>
#include "Headers/manager.h"
#include "Headers/employee.h"
#include "Headers/client.h"
#include "Headers/account.h"
#include "Headers/facilities.h"
#include "Headers/bank.h"
#include "Headers/helperClass.h"
using namespace std;

void printMainMenu();
void mainMenu();

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
void mainMenu() {

}

void clearConsole() {
    if(system("clear") == 0) return;
    system("cls");
}
void awaitKey() {
    system("pause");
}