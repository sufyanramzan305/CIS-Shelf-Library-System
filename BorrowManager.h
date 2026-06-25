#pragma once
#define NOMINMAX
#include <windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>
#include <stdexcept>
#include <ctime>
#include "Borrow_Record.h"
#include "Resource.h"
#include "User.h"
#include "UserManager.h"
#include "ResourceManager.h"
using namespace std;

void setColor(int color);

class BorrowManager
{
private:
    vector<Borrow_Record> records;
    const string filename = "records.txt";

public:
    void saveToFile();
    void loadFromFile();

    void borrow_resource(User *user, vector<Resource> &resources,
                         UserManager &um, ResourceManager &rm);

    void returnResource(User *user, vector<Resource> &resources,
                        UserManager &um, ResourceManager &rm);

    void check_User_Fine(User *user);
    void printAllRecords();
};