#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <stdexcept>
using namespace std;

class Borrow_Record
{
private:
    string username;
    int resourceId;
    time_t dueDate;
    time_t issueDate;
    bool returned;

public:
    Borrow_Record(string u = "", int rd = 0);
    Borrow_Record(string u, int rd, time_t issue, time_t due, bool ret);

    string getUser() const;
    int getId() const;
    int getResourceID() const;
    bool isReturned() const;

    double Fine_calculation() const;
    void mark_as_returned();
    void saveToFile(ofstream &out) const;
    void Display() const;
};