#pragma once
#include "User.h"
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

class UserManager
{
private:
    vector<User> users;
    const string filename = "users.txt";

public:
    void saveToFile();
    void loadFromFile();
    void register_user();
    string login_user();

    User *findUser(const string &username);
    void printAllUsers();

    vector<User> &getUsers();
    void syncFile();
};