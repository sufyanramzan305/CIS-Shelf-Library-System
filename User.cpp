#include "User.h"
#include <stdexcept>

User::User(const string &u, const string &p, const string &f,
           const string &l, const string &a, const string &e)
    : username(u), password(p), first_name(f),
      last_name(l), address(a), email(e),
      balance(Default_balance), borrowed_count(0) {}

User::User(const string &u, const string &p, const string &f,
           const string &l, const string &a, const string &e,
           double bal, int bc)
    : username(u), password(p), first_name(f),
      last_name(l), address(a), email(e),
      balance(bal), borrowed_count(bc) {}

string User::get_Username() const { return username; }
string User::getEmail() const { return email; }
double User::getBalance() const { return balance; }

bool User::check_password(const string &entered_password) const
{
    return password == entered_password;
}

bool User::can_borrow() const { return borrowed_count < Max_borrow_limit; }

void User::increment_borrow()
{
    if (can_borrow())
        borrowed_count++;
}

void User::decrement_borrow()
{
    if (borrowed_count > 0)
        borrowed_count--;
}

void User::deduct_fine(double fine)
{
    if (fine > 0)
    {
        if (fine > balance)
            throw runtime_error("Fine exceeds available balance!");
        balance -= fine;
    }
}

void User::saveToFile(ofstream &out) const
{
    out << username << "|" << password << "|" << first_name << "|"
        << last_name << "|" << address << "|" << email << "|"
        << balance << "|" << borrowed_count << "\n";
}

void User::Display() const
{
    cout << "-----USER INFO-------\n"
         << "USERNAME: " << username
         << " | NAME: " << first_name << " " << last_name
         << " | BALANCE: " << balance
         << " | BORROWED BOOKS: " << borrowed_count << "\n";
}