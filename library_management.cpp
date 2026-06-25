#define NOMINMAX
#include <windows.h> //colouring
#include <iostream>
#include <fstream> //file handling
#include <sstream>
#include <string>
#include <vector> //dynamic array
#include <ctime>  //date and time
#include <algorithm>
#include <limits>
#include <stdexcept> //exception handling
using namespace std;

void setColor(int color) // appearance of text on terminal
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

//==========================================================//
//-------------------- LIBRARYENTITY CLASS -----------------//
// Abstract base class — enforces Display() on all entities //
//==========================================================//

class LibraryEntity
{
public:
    // Pure virtual function — makes LibraryEntity an abstract class
    // Any class inheriting this MUST implement its own Display()
    virtual void Display() const = 0;

    // Virtual destructor — good practice for polymorphic base classes
    virtual ~LibraryEntity() {} 
};

//==========================================================//
//--------------------------RESOURCE CLASS-----------------//
// Inherits from LibraryEntity (Inheritance)               //
// Overrides Display() (Method Overriding)                 //
//==========================================================//

class Resource : public LibraryEntity // INHERITANCE from abstract base class
{
private: // data hiding
    int id;
    string title;
    string author;
    string category;
    bool available;

public:
    Resource() : id(0), title(""), author(""), category(""), available(true) {}

    Resource(int i, string t, string a, string c)
        : id(i), title(t), author(a), category(c), available(true) {}

    int getId() const { return id; } // accessors (read and return)
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    string getCategory() const { return category; }
    bool isAvailable() const { return available; } // return 1 or 0

    void setAvailability(bool status) { available = status; } // mutator

    // OPERATOR OVERLOADING: compare two Resources by their ID
    // Useful when searching or matching resources by ID
    bool operator==(const Resource &other) const
    {
        return this->id == other.id;
    }

    // OPERATOR OVERLOADING: compare Resource directly against an integer ID
    // Allows: if (resource == 5) instead of if (resource.getId() == 5)
    bool operator==(int otherId) const
    {
        return this->id == otherId;
    }

    void saveToFile(ofstream &out) const
    {
        out << id << "|" << title << "|" << author << "|" // Format: id|title|author|category|available
            << category << "|" << available << "\n";
    }

    // METHOD OVERRIDING: overrides pure virtual Display() from LibraryEntity
    // 'override' keyword confirms this is intentionally overriding a base method
    void Display() const override
    {
        cout << id << " | " << title << " | " << author << " | "
             << category << " | AVAILABLE: " << (available ? "YES" : "NO") << endl;
    }

    // Colored display variant (used by ResourceManager for UI)
    void Display(int color) const
    {
        setColor(color);
        Display(); // calls the overridden Display()
        setColor(15);
    }
};

//==========================================================//
//----------------------BORROW_RECORD CLASS-----------------//
//==========================================================//

class Borrow_Record
{
private:
    string username;
    int resourceId;
    time_t dueDate;
    time_t issueDate;
    bool returned;

public:
    // Constructor used when borrowing a new book
    Borrow_Record(string u = "", int rd = 0) : username(u), resourceId(rd) // default + parametrized
    {
        issueDate = time(0); // gets current time

        dueDate = issueDate + 604800; // 7 days * 24 hours * 60 mins * 60 sec
        // fine will be imposed after 7 days of issue date

        returned = false;
    }

    // Constructor used when loading from file
    Borrow_Record(string u, int rd, time_t issue, time_t due, bool ret)
        : username(u), resourceId(rd), issueDate(issue), dueDate(due), returned(ret) {}

    string getUser() const { return username; }
    int getId() const { return resourceId; }
    int getResourceID() const { return resourceId; }
    bool isReturned() const { return returned; }

    double Fine_calculation() const
    {
        time_t Now = time(0);
        if (Now == -1) // error check for system clock failure
            throw runtime_error("System clock is not working properly!");

        if (Now > dueDate && !returned)
        {
            double daysLate = (Now - dueDate) / 86400.0; // subtract dueDate from Now to get seconds overdue
            // fine imposed even after one hour late
            return daysLate * 10;
        }
        return 0;
    }

    void mark_as_returned() { returned = true; }

    // Format: username|resourceId|issueDate|dueDate|returned
    void saveToFile(ofstream &out) const
    {
        out << username << "|" << resourceId << "|"
            << issueDate << "|" << dueDate << "|" << returned << "\n";
    }

    void Display() const
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
};

//==========================================================//
//----------------------USER CLASS--------------------------//
// Inherits from LibraryEntity (Inheritance)               //
// Overrides Display() (Method Overriding)                 //
//==========================================================//

class User : public LibraryEntity // INHERITANCE from abstract base class
{
private:
    string username, password, first_name, last_name, address, email;
    double balance;
    int borrowed_count;
    static const int Max_borrow_limit = 2;
    static constexpr double Default_balance = 1000.0;

public:
    User(const string &u, const string &p, const string &f,
         const string &l, const string &a, const string &e)
        : username(u), password(p), first_name(f),
          last_name(l), address(a), email(e),
          balance(Default_balance), borrowed_count(0) {}

    // Constructor used when loading from file
    User(const string &u, const string &p, const string &f,
         const string &l, const string &a, const string &e,
         double bal, int bc)
        : username(u), password(p), first_name(f),
          last_name(l), address(a), email(e),
          balance(bal), borrowed_count(bc) {}

    string get_Username() const { return username; }

    bool check_password(const string &entered_password) const
    {
        return password == entered_password;
    }

    bool can_borrow() const { return borrowed_count < Max_borrow_limit; }

    void increment_borrow()
    {
        if (can_borrow())
            borrowed_count++;
    }

    void decrement_borrow()
    {
        if (borrowed_count > 0)
            borrowed_count--;
    }

    void deduct_fine(double fine)
    {
        // Only attempt deduction if there is actually a fine to pay
        if (fine > 0)
        {
            // Exception handling: throw if fine exceeds available balance
            // This protects the balance from going negative
            if (fine > balance)
                throw runtime_error("Fine exceeds available balance!");

            balance -= fine; // deduct fine from balance if sufficient funds exist
        }
    }

    double getBalance() const { return balance; } // accessor to read current balance

    string getEmail() const { return email; }

    // Format: username|password|first|last|address|email|balance|borrowed_count
    void saveToFile(ofstream &out) const
    {
        out << username << "|" << password << "|" << first_name << "|"
            << last_name << "|" << address << "|" << email << "|"
            << balance << "|" << borrowed_count << "\n";
    }

    // METHOD OVERRIDING: overrides pure virtual Display() from LibraryEntity
    // 'override' keyword confirms this is intentionally overriding a base method
    void Display() const override
    {
        cout << "-----USER INFO-------\n"
             << "USERNAME: " << username
             << " | NAME: " << first_name << " " << last_name
             << " | BALANCE: " << balance
             << " | BORROWED BOOKS: " << borrowed_count << "\n";
    }
};

//==========================================================//
//------------------ RESOURCE_MANAGER CLASS-----------------//
//==========================================================//

class ResourceManager
{
private:
    vector<Resource> resource;
    const string filename = "C:\\Users\\3tee\\OneDrive\\Desktop\\G4(6)\\resources.txt"; // resources file

public:
    void saveToFile()
    {
        ofstream out(filename);
        for (auto &r : resource)
            r.saveToFile(out);
    }

    void loadFromFile()
    {
        ifstream in(filename);
        if (!in)
            return;

        string line;
        while (getline(in, line))
        {
            if (line.empty())
                continue;
            stringstream ss(line);

            string sid, title, author, category, avail;
            getline(ss, sid, '|');
            getline(ss, title, '|');
            getline(ss, author, '|');
            getline(ss, category, '|');
            getline(ss, avail, '|');

            int id = stoi(sid);
            bool available = (avail == "1");

            resource.emplace_back(id, title, author, category);
            resource.back().setAvailability(available);
        }
    }

    void add_Resource()
    {
        string idInput, Title, Author, Category;

        cout << "ID: ";
        getline(cin, idInput);

        int id;
        try // exception handling: catch non-numeric ID input
        {
            id = stoi(idInput);
        }
        catch (invalid_argument &)
        {
            setColor(4);
            cout << "ERROR: ID must be a number! Resource not added.\n";
            setColor(15);
            return;
        }

        cout << "TITLE: ";
        getline(cin, Title);
        cout << "AUTHOR: ";
        getline(cin, Author);
        cout << "CATEGORY: ";
        getline(cin, Category);

        resource.emplace_back(id, Title, Author, Category);
        saveToFile();
        cout << "RESOURCE ADDED SUCCESSFULLY\n";
    }

    void display_availableResource()
    {
        int bookColors[] = {11, 14, 10, 9, 6, 3, 7, 15};
        int numColors = sizeof(bookColors) / sizeof(bookColors[0]);
        int colorIndex = 0;

        for (auto &r : resource)
        {
            if (r.isAvailable())
            {
                r.Display(bookColors[colorIndex % numColors]);
                colorIndex++;
            }
        }
    }

    void search_by_title()
    {
        string searchTitle;
        cout << "Enter book title (or part of title): ";
        getline(cin, searchTitle);

        transform(searchTitle.begin(), searchTitle.end(), searchTitle.begin(), ::tolower);

        int bookColors[] = {11, 14, 10, 9, 6, 3, 7, 15};
        int numColors = sizeof(bookColors) / sizeof(bookColors[0]);
        int colorIndex = 0;
        bool found = false;

        for (auto &r : resource)
        {
            string bookTitle = r.getTitle();
            transform(bookTitle.begin(), bookTitle.end(), bookTitle.begin(), ::tolower);

            if (bookTitle.find(searchTitle) != string::npos)
            {
                r.Display(bookColors[colorIndex % numColors]);
                colorIndex++;
                found = true;
            }
        }

        if (!found)
            cout << "No book found with this title.\n";
    }

    void delete_Resource()
    {
        int id;
        cout << "Enter Resource ID to delete: ";
        cin >> id;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        for (auto it = resource.begin(); it != resource.end(); ++it)
        {
            // Using overloaded operator== to compare Resource with integer ID
            if (*it == id)
            {
                if (!it->isAvailable())
                {
                    cout << "Cannot delete. Book is currently borrowed.\n";
                    return;
                }
                resource.erase(it);
                saveToFile();
                cout << "Resource deleted successfully.\n";
                return;
            }
        }
        cout << "Resource not found.\n";
    }

    void addDefaultResources()
    {
        if (!resource.empty())
            return;

        resource.emplace_back(1, "Harry Potter", "J.K. Rowling", "Fantasy");
        resource.emplace_back(2, "The Alchemist", "Paulo Coelho", "Fiction");
        resource.emplace_back(3, "Clean Code", "Robert C. Martin", "Programming");
        resource.emplace_back(4, "Introduction to Algorithms", "Cormen et al.", "Programming");
        resource.emplace_back(5, "Pride and Prejudice", "Jane Austen", "Classic");
        saveToFile();
    }

    vector<Resource> &getResources() { return resource; }
    void syncFile() { saveToFile(); }
};

//==========================================================//
//----------------------USER_MANAGER CLASS-----------------//
//==========================================================//

class UserManager
{
private:
    vector<User> users;
    const string filename = "C:\\Users\\3tee\\OneDrive\\Desktop\\G4(6)\\users.txt";

public:
    void saveToFile()
    {
        ofstream out(filename);
        for (auto &u : users)
            u.saveToFile(out);  
        }

    void loadFromFile()
    {
        // Clear existing users before reloading to avoid duplicates
        users.clear();

        ifstream in(filename);
        if (!in)
            return;

        string line;
        while (getline(in, line))
        {
            if (line.empty())
                continue;
            stringstream ss(line);

            string u, p, f, l, a, e, bal, bc;
            getline(ss, u, '|');
            getline(ss, p, '|');
            getline(ss, f, '|');
            getline(ss, l, '|');
            getline(ss, a, '|');
            getline(ss, e, '|');
            getline(ss, bal, '|');
            getline(ss, bc, '|');

            users.emplace_back(u, p, f, l, a, e, stod(bal), stoi(bc));
        }
    }

    void register_user()
    {
        string u, p, f, l, a, e;

        cout << "USERNAME: ";
        getline(cin, u);
        cout << "PASSWORD: ";
        getline(cin, p);
        cout << "FIRST NAME: ";
        getline(cin, f);
        cout << "LAST NAME: ";
        getline(cin, l);
        cout << "ADDRESS: ";
        getline(cin, a);
        cout << "EMAIL: ";
        getline(cin, e);

        users.emplace_back(u, p, f, l, a, e);
        saveToFile();
        cout << "ACCOUNT SUCCESSFULLY CREATED\n";

        auto sanitise = [](string s)
        {
            string out;
            for (char c : s)
                if (c != '"' && c != '\'' && c != '\\' && c != '&' &&
                    c != '|' && c != ';' && c != '`' && c != '<' && c != '>')
                    out += c;
            return out;
        };

        string su = sanitise(u), sf = sanitise(f), sl = sanitise(l),
               sa = sanitise(a), se = sanitise(e);

        string cmd = "cmd /c python send_email.py \"" + su + "\" \"" + sf + "\" \"" +
                     sl + "\" \"" + sa + "\" \"" + se + "\"";
        int result = system(cmd.c_str());
        if (result != 0)
            cout << "Failed to send email notification. Check internet or Python script.\n";
    }

    string login_user()
    {
        string u, p;
        cout << "ENTER USERNAME: ";
        getline(cin, u);
        cout << "ENTER PASSWORD: ";
        getline(cin, p);

        for (int i = 0; i < (int)users.size(); i++)
        {
            if (users[i].get_Username() == u && users[i].check_password(p))
            {
                cout << "LOGIN SUCCESSFUL\n";
                return u;
            }
        }

        cout << "INVALID LOGIN\n";
        return "";
    }

    User *findUser(const string &username)
    {
        for (auto &u : users)
            if (u.get_Username() == username)
                return &u;
        return nullptr;
    }

    void printAllUsers()
    {
        for (auto &u : users)
            u.Display(); // calls User::Display() — overridden method
    }

    vector<User> &getUsers() { return users; }
    void syncFile() { saveToFile(); }
};

//==========================================================//
//---------------------BORROW_MANAGER CLASS-----------------//
//==========================================================//

class BorrowManager
{
private:
    vector<Borrow_Record> records;
    const string filename = "C:\\Users\\3tee\\OneDrive\\Desktop\\G4(6)\\records.txt";

public:
    void saveToFile()
    {
        ofstream out(filename);
        for (auto &r : records)
            r.saveToFile(out);
    }

    void loadFromFile()
    {
        ifstream in(filename);
        if (!in)
            return;

        string line;
        while (getline(in, line))
        {
            if (line.empty())
                continue;
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

    void borrow_resource(User *user, vector<Resource> &resources,
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
        try // exception handling: catch non-numeric resource ID input
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
            // Using overloaded operator== to compare Resource with integer ID
            if (r == id && r.isAvailable())
            {
                r.setAvailability(false);                       // mark resource as unavailable
                records.emplace_back(user->get_Username(), id); // create new borrow record
                user->increment_borrow();                       // increase user's borrow count

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

    void returnResource(User *user, vector<Resource> &resources,
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

                try // exception handling: catch system clock failure during fine calculation
                {
                    fine = rec.Fine_calculation(); // calculate fine based on overdue days
                }
                catch (runtime_error &e)
                {
                    setColor(4);
                    cout << "WARNING: " << e.what() << " Fine set to 0.\n";
                    setColor(15);
                }

                rec.mark_as_returned();   // mark borrow record as returned
                user->decrement_borrow(); // reduce user's active borrow count

                try // exception handling: fine may exceed user balance
                {
                    user->deduct_fine(fine); // attempt to deduct fine from user balance

                    setColor(10);
                    cout << "Fine deducted successfully. Fine: " << fine << " Rupees\n";
                    setColor(15);
                }
                catch (runtime_error &e)
                {
                    // Fine exceeds balance — show error, mark user as defaulter, do NOT deduct
                    setColor(4);
                    cout << "\n\t!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
                    cout << "\t         *** DEFAULTER USER ***      \n";
                    cout << "\t!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
                    cout << "ERROR: " << e.what() << "\n";
                    cout << "Your Balance  : Rs. " << user->getBalance() << "\n";
                    cout << "Fine Amount   : Rs. " << fine << "\n";
                    cout << "Extra Amount  : Rs. " << (fine - user->getBalance())
                         << " (Pay remaining at counter)\n";
                    cout << "Balance has NOT been deducted.\n";
                    setColor(15);
                }

                // Mark resource as available again after return
                for (auto &r : resources)
                {
                    // Using overloaded operator== to compare Resource with integer ID
                    if (r == id)
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

    void check_User_Fine(User *user)
    {
        double totalFine = 0;

        for (auto &rec : records)
        {
            if (rec.getUser() == user->get_Username() && !rec.isReturned())
            {
                try // exception handling: catch system clock failure during fine check
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

    void printAllRecords()
    {
        for (auto &r : records)
            r.Display();
    }
};

//==========================================================//
//==========================================================//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//-----------------LIBRARY_MANAGEMENT CLASS-----------------//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//==========================================================//
//==========================================================//

class Library_Management
{
private:
    ResourceManager resourceManager;
    UserManager userManager;
    BorrowManager borrowManager;

public:
    void loadAllData()
    {
        resourceManager.loadFromFile();
        userManager.loadFromFile();
        borrowManager.loadFromFile();
    }

    void userMenu(User *user)
    {
        string username = user->get_Username();
        int choice;
        do
        {
            user = userManager.findUser(username); // re-fetch user pointer each loop to reflect updated data
            if (!user)
            {
                cout << "Session error. Logging out.\n";
                break;
            }

            setColor(3);
            cout << "\n\t=========================================\n";
            setColor(11);
            cout << "\t               USER MENU                 \n";
            setColor(3);
            cout << "\t=========================================\n";
            setColor(6);
            cout << "\t[1] View Available Resources\n";
            cout << "\t[2] Search by Title\n";
            cout << "\t[3] Borrow Resource\n";
            cout << "\t[4] Return Resource\n";
            cout << "\t[5] Check Fine\n";
            cout << "\t[6] Logout\n";
            setColor(3);
            cout << "\t-----------------------------------------\n";
            setColor(15);
            cout << "\tEnter your choice: ";

            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (choice)
            {
            case 1:
                setColor(11);
                cout << "\n\t--- Available Resources ---\n";
                setColor(15);
                resourceManager.display_availableResource();
                break;
            case 2:
                setColor(11);
                cout << "\n\t--- Search Resource ---\n";
                setColor(15);
                resourceManager.search_by_title();
                break;
            case 3:
                setColor(14);
                cout << "\n\t--- Borrow Resource ---\n";
                setColor(15);
                borrowManager.borrow_resource(user, resourceManager.getResources(),
                                              userManager, resourceManager);
                break;
            case 4:
                setColor(14);
                cout << "\n\t--- Return Resource ---\n";
                setColor(15);
                borrowManager.returnResource(user, resourceManager.getResources(),
                                             userManager, resourceManager);
                break;
            case 5:
                setColor(11);
                cout << "\n\t--- Fine Details ---\n";
                setColor(15);
                borrowManager.check_User_Fine(user);
                break;
            case 6:
                setColor(11);
                cout << "\n\tLogging out...\n";
                setColor(15);
                break;
            default:
                setColor(4);
                cout << "\n\tInvalid choice.\n";
                setColor(15);
            }

        } while (choice != 6);
    }

    void adminMenu()
    {
        int choice;
        do
        {
            setColor(3);
            cout << "\n\t=========================================\n";
            setColor(13);
            cout << "\t             ADMIN PANEL                 \n";
            setColor(3);
            cout << "\t=========================================\n";
            setColor(5);
            cout << "\t[1] Add Resource\n";
            cout << "\t[2] Delete Resource\n";
            cout << "\t[3] View All Users\n";
            cout << "\t[4] View All Records\n";
            cout << "\t[5] Exit Panel\n";
            setColor(3);
            cout << "\t-----------------------------------------\n";
            setColor(15);
            cout << "\tEnter your choice: ";

            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (choice)
            {
            case 1:
                setColor(14);
                cout << "\n\t--- Add Resource ---\n";
                setColor(15);
                resourceManager.add_Resource();
                break;
            case 2:
                setColor(4);
                cout << "\n\t--- Delete Resource ---\n";
                setColor(15);
                resourceManager.delete_Resource();
                break;
            case 3:
                setColor(11);
                cout << "\n\t--- All Users ---\n";
                setColor(15);
                userManager.printAllUsers(); // calls overridden User::Display() for each user
                break;
            case 4:
                setColor(11);
                cout << "\n\t--- All Borrow Records ---\n";
                setColor(15);
                borrowManager.printAllRecords();
                break;
            case 5:
                setColor(11);
                cout << "\n\tExiting Admin Panel...\n";
                setColor(15);
                break;
            default:
                setColor(4);
                cout << "\n\tInvalid choice!\n";
                setColor(15);
            }

        } while (choice != 5);
    }

    void clearScreen() { system("cls"); }
    void pause() { system("pause"); }

    void mainMenu()
    {
        int choice;

        loadAllData();                         // load all saved data from files on startup
        resourceManager.addDefaultResources(); // add default books if no resources exist

        do
        {
            clearScreen();

            setColor(3);
            cout << "\n\t=========================================\n";
            setColor(11);
            cout << "\t               CIS SHELF             \n";
            setColor(3);
            cout << "\t=========================================\n";
            setColor(6);
            cout << "\n\t[1] Register";
            cout << "\n\t[2] Login";
            cout << "\n\t[3] Librarian Panel";
            cout << "\n\t[4] Exit";
            setColor(3);
            cout << "\n\n\t-----------------------------------------\n";
            setColor(15);
            cout << "\tEnter your choice: ";

            cin >> choice;

            if (cin.fail()) // exception handling: catch invalid non-numeric menu input
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                setColor(4);
                cout << "\n\tInvalid input! Please enter a number.\n";
                setColor(15);
                pause();
                continue;
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (choice)
            {
            case 1:
                clearScreen();
                setColor(3);
                cout << "\n\t=========================================\n";
                setColor(11);
                cout << "\t             REGISTER USER               \n";
                setColor(3);
                cout << "\t=========================================\n";
                setColor(15);
                userManager.register_user();
                break;

            case 2:
            {
                clearScreen();
                setColor(3);
                cout << "\n\t=========================================\n";
                setColor(11);
                cout << "\t               USER LOGIN                \n";
                setColor(3);
                cout << "\t=========================================\n";
                setColor(15);

                string loggedInUsername = userManager.login_user();
                if (!loggedInUsername.empty())
                {
                    setColor(10);
                    cout << "\n\tLogin successful!\n";
                    setColor(15);
                    pause();
                    User *user = userManager.findUser(loggedInUsername);
                    if (user)
                        userMenu(user);
                }
                else
                {
                    setColor(4);
                    cout << "\n\tLogin failed! Invalid username or password.\n";
                    setColor(15);
                }
                break;
            }

            case 3:
                clearScreen();
                setColor(3);
                cout << "\n\t=========================================\n";
                setColor(13);
                cout << "\t            LIBRARIAN PANEL              \n";
                setColor(3);
                cout << "\t=========================================\n";
                setColor(15);
                adminMenu();
                break;

            case 4:
                setColor(11);
                cout << "\n\tExiting system... Goodbye!\n";
                setColor(15);
                break;

            default:
                setColor(4);
                cout << "\n\tInvalid choice! Try again.\n";
                setColor(15);
            }

            pause();

        } while (choice != 4);
    }
};

//==========================================================//
//-----------------------------MAIN-------------------------//
//==========================================================//

int main()
{
    Library_Management system;
    system.mainMenu(); // start the library system
    return 0;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//