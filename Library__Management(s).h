#pragma once
#include "ResourceManager.h"
#include "UserManager.h"
#include "BorrowManager.h"
using namespace std;

class Library_Management
{
private:
    ResourceManager resourceManager;
    UserManager userManager;
    BorrowManager borrowManager;

public:
    void loadAllData();
    void userMenu(User *user);
    void adminMenu();
    void clearScreen();
    void pause();
    void mainMenu();
};