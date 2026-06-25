#include "Borrow_Record.h"

Borrow_Record::Borrow_Record(string u, int rd) : username(u), resourceId(rd)
{
    issueDate = time(0);
    dueDate = issueDate + 604800;
    returned = false;
}

Borrow_Record::Borrow_Record(string u, int rd, time_t issue, time_t due, bool ret)
    : username(u), resourceId(rd), issueDate(issue), dueDate(due), returned(ret) {}

string Borrow_Record::getUser() const { return username; }
int Borrow_Record::getId() const { return resourceId; }
int Borrow_Record::getResourceID() const { return resourceId; }
bool Borrow_Record::isReturned() const { return returned; }

double Borrow_Record::Fine_calculation() const
{
    time_t Now = time(0);
    if (Now == -1)
        throw runtime_error("System clock is not working properly!");

    if (Now > dueDate && !returned)
    {
        double daysLate = (Now - dueDate) / 86400.0;
        return daysLate * 10;
    }
    return 0;
}

void Borrow_Record::mark_as_returned() { returned = true; }

void Borrow_Record::saveToFile(ofstream &out) const
{
    out << username << "|" << resourceId << "|"
        << issueDate << "|" << dueDate << "|" << returned << "\n";
}

void Borrow_Record::Display() const
{
    char issueBuffer[80], dueBuffer[80];
    struct tm issueTm = *localtime(&issueDate);
    struct tm dueTm = *localtime(&dueDate);
    strftime(issueBuffer, sizeof(issueBuffer), "%Y-%m-%d %H:%M:%S", &issueTm);
    strftime(dueBuffer, sizeof(dueBuffer), "%Y-%m-%d %H:%M:%S", &dueTm);

    cout << "----------USER RECORD---------\n"
         << "USER: " << username << "\n"
         << "RESOURCE ID: " << resourceId << "\n"
         << "ISSUE DATE: " << issueBuffer << "\n"
         << "DUE DATE:  " << dueBuffer << "\n"
         << "FINE: " << Fine_calculation() << " Rupees\n"
         << "STATUS: " << (returned ? " RETURNED " : " ACTIVE ") << "\n";
}