#include "BorrowManager.h"

void BorrowManager::saveToFile()
{
    ofstream out(filename);
    for (auto &r : records)
        r.saveToFile(out);
}

void BorrowManager::loadFromFile()
{
    ifstream in(filename);
    if (!in) return;

    string line;
    while (getline(in, line))
    {
        if (line.empty()) continue;
        stringstream ss(line);

        string u, rid, issue, due, ret;
        getline(ss, u, '|');
        getline(ss, rid, '|');
        getline(ss, issue, '|');
        getline(ss, due, '|');
        getline(ss, ret, '|');

        records.emplace_back(u, stoi(rid),
                             (time_t)stoll(issue),
                             (time_t)stoll(due),
                             ret == "1");
    }
}

void BorrowManager::borrow_resource(User *user, vector<Resource> &resources,
                                     UserManager &um, ResourceManager &rm)
{
    if (!user->can_borrow())
    {
        setColor(4);
        cout << "BORROW LIMIT REACHED\n";
        setColor(15);
        return;
    }

    string idInput;
    cout << "ENTER THE ID: ";
    getline(cin, idInput);

    int id;
    try
    {
        id = stoi(idInput);
    }
    catch (invalid_argument &)
    {
        setColor(4);
        cout << "ERROR: Please enter a valid number for the ID!\n";
        setColor(15);
        return;
    }

    for (auto &r : resources)
    {
        if (r.getId() == id && r.isAvailable())
        {
            r.setAvailability(false);
            records.emplace_back(user->get_Username(), id);
            user->increment_borrow();

            saveToFile();
            rm.syncFile();
            um.syncFile();

            setColor(10);
            cout << "Issued Successfully.\n";
            setColor(15);
            return;
        }
    }
    setColor(4);
    cout << "NOT AVAILABLE\n";
    setColor(15);
}

void BorrowManager::returnResource(User *user, vector<Resource> &resources,
                                    UserManager &um, ResourceManager &rm)
{
    int id;
    cout << "Enter Resource ID: ";
    cin >> id;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (auto &rec : records)
    {
        if (rec.getUser() == user->get_Username() &&
            rec.getResourceID() == id &&
            !rec.isReturned())
        {
            double fine = 0;

            try
            {
                fine = rec.Fine_calculation();
            }
            catch (runtime_error &e)
            {
                setColor(4);
                cout << "WARNING: " << e.what() << " Fine set to 0.\n";
                setColor(15);
            }

            rec.mark_as_returned();
            user->decrement_borrow();

            try
            {
                user->deduct_fine(fine);
                setColor(10);
                cout << "Fine deducted successfully. Fine: " << fine << " Rupees\n";
                setColor(15);
            }
            catch (runtime_error &e)
            {
                setColor(4);
                cout << "\n\t!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
                cout << "\t         *** DEFAULTER USER ***      \n";
                cout << "\t!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
                cout << "ERROR: " << e.what() << "\n";
                cout << "Your Balance  : Rs. " << user->getBalance() << "\n";
                cout << "Fine Amount   : Rs. " << fine << "\n";
                cout << "Extra Amount  : Rs. " << (fine - user->getBalance()) << " (Pay remaining at counter)\n";
                cout << "Balance has NOT been deducted.\n";
                setColor(15);
            }

            for (auto &r : resources)
            {
                if (r.getId() == id)
                {
                    r.setAvailability(true);
                    break;
                }
            }

            saveToFile();
            rm.syncFile();
            um.syncFile();

            setColor(10);
            cout << "Returned Successfully.\n";
            setColor(15);
            return;
        }
    }
    setColor(4);
    cout << "No matching borrow record found.\n";
    setColor(15);
}

void BorrowManager::check_User_Fine(User *user)
{
    double totalFine = 0;

    for (auto &rec : records)
    {
        if (rec.getUser() == user->get_Username() && !rec.isReturned())
        {
            try
            {
                totalFine += rec.Fine_calculation();
            }
            catch (runtime_error &e)
            {
                setColor(4);
                cout << "WARNING: " << e.what() << "\n";
                setColor(15);
            }
        }
    }

    setColor(3);
    cout << "\n\t===== FINE DETAILS =====\n";
    setColor(15);

    if (totalFine == 0)
    {
        setColor(10);
        cout << "Total Fine: 0 Rupees\n";
        cout << "You are within the 7-day borrowing period.\n";
        setColor(15);
    }
    else
    {
        setColor(4);
        cout << "Total Fine: " << totalFine << " Rupees\n";
        cout << "Book is overdue!\n";
        setColor(15);
    }
}

void BorrowManager::printAllRecords()
{
    for (auto &r : records)
        r.Display();
}