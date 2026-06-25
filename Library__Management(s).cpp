#include "Library__Management(s).h"
#include <limits>

void Library_Management::loadAllData()
{
    resourceManager.loadFromFile();
    userManager.loadFromFile();
    borrowManager.loadFromFile();
}

void Library_Management::userMenu(User *user)
{
    string username = user->get_Username();
    int choice;
    do
    {
        user = userManager.findUser(username);
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

void Library_Management::adminMenu()
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
            userManager.printAllUsers();
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

void Library_Management::clearScreen() { system("cls"); }
void Library_Management::pause() { system("pause"); }

void Library_Management::mainMenu()
{
    int choice;

    loadAllData();
    resourceManager.addDefaultResources();

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

        if (cin.fail())
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