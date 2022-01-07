#include <iostream>
#include "Headers/manager.h"
#include "Headers/employee.h"
#include "Headers/client.h"
#include "Headers/account.h"
#include "Headers/facilities.h"
#include "Headers/bank.h"
using namespace std;


int main() {
    Manager manager{"Alireza", "Kamyab", "1850484211", 102, Date{27, 4, 1381}, "aimlessly", "123456789"};
    system("pause");
    return 0;
}