#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "LibraryEntity.h"
using namespace std;

class User : public LibraryEntity
{
private:
    string username, password, first_name, last_name, address, email;
    double balance;
    int borrowed_count;
    static const int Max_borrow_limit = 2;
    static constexpr double Default_balance = 1000.0;

public:
    // Constructor for new user
    User(const string &u, const string &p, const string &f,
         const string &l, const string &a, const string &e);

    // Constructor for loading from file
    User(const string &u, const string &p, const string &f,
         const string &l, const string &a, const string &e,
         double bal, int bc);

    string get_Username() const;
    string getEmail() const;
    double getBalance() const;

    bool check_password(const string &entered_password) const;
    bool can_borrow() const;

    void increment_borrow();
    void decrement_borrow();
    void deduct_fine(double fine);

    void saveToFile(ofstream &out) const;
    void Display() const override;
};