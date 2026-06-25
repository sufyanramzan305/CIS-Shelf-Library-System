================================================================================
================================================================================
                        CIS SHELF — E-Library Management System
--------------------------------------------------------------------------------
                        C++ Object-Oriented Programming Project
================================================================================
================================================================================

                      WELCOME TO THE CIS SHELF README FILE

--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
1. TEAM MEMBERS
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------

  Name              : Daniyal Ahmed
  Roll Number       : CS-25134
  Section           : C

  Name              : Muhammad Shariq
  Roll Number       : CS-25140
  Section           : C

  Name              : Hayan Khan
  Roll Number       : CS-25147
  Section           : C

  Name              : Sufyan Ramzan
  Roll Number       : CS-25135
  Section           : C

  Batch             : 2025-2029
  Semester          : Spring (May)
  Room No           : CS-4
  CEP Group ID      : G4-6

--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
2. STEPS TO RUN THE PROGRAM
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------

========================================
--- RUNNING THE GUI VERSION (Python) ---
========================================

  Step 1: First we have set the batch file of GUI on the desktop. Click on
          the .bat file to open the GUI.

  Step 2: The GUI will now open. Register completely by filling in your
          username, password, first name, last name, address, and Gmail ID.

  Step 3: Click on the registration button. It will make you wait for 1 to
          2 seconds and a message will appear on your screen: "PDF sent."

  Step 4: Click OK. It will send a confirmation email to certain registered
          email addresses such as those created by Daniyal Ahmed.

  Step 5: Registration is now complete. You will be notified on your email
          with a message present in REPORT.pdf and a PDF file containing
          your basic info such as username, first name, last name, address,
          Gmail ID, and the starting balance of 1000 Rs.

============================================
  --- RUNNING THE CONSOLE VERSION (C++) ---
============================================

  Step 1: Open the batch file on the desktop. The batch file is created
          for convenience so the program does not need to be compiled
          manually each time.

  Step 2: The CMD terminal will open with the first interface of the
          library system.

  Step 3: The main menu will appear with the following options:
            [1] Register
            [2] Login
            [3] Librarian Panel
            [4] Exit

  Step 4: If already registered, enter your USERNAME and PASSWORD.
          The user menu will then open.

  NOTE: Data files (users.txt, resources.txt, records.txt) are saved
        to a fixed path defined in the source code:
        C:\Users\3tee\OneDrive\Desktop\G4(6)\
        Do not delete these files or all borrowing and user data will
        be lost. If running on a different machine, update the file
        paths in the source code before compiling.

--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
3. OOP CONCEPTS — CLASS / FUNCTION MAPPING
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------

  CONCEPT                    | CLASS / FUNCTION THAT DEMONSTRATES IT
  ---------------------------|--------------------------------------------------
  Encapsulation              | Resource, User, Borrow_Record
                             | All data members are private. Access is provided
                             | through public getters/setters such as getTitle(),
                             | get_Username(), isAvailable(), and getEmail().
  ---------------------------|--------------------------------------------------
  Abstraction                | Library_Management, UserManager, BorrowManager
                             | Internal logic (file handling, authentication,
                             | fine calculation) is hidden from the user.
                             | Users interact only through simple menus.
  ---------------------------|--------------------------------------------------
  Inheritance                | Resource : public LibraryEntity
                             | User : public LibraryEntity
                             | Both classes inherit from the abstract base
                             | class LibraryEntity using public inheritance.
  ---------------------------|--------------------------------------------------
  Abstract Base Class        | LibraryEntity
                             | Contains pure virtual function:
                             | virtual void Display() const = 0;
                             | This makes LibraryEntity an abstract class
                             | and forces derived classes to implement
                             | their own Display() method.
  ---------------------------|--------------------------------------------------
  Polymorphism               | Resource::Display()
                             | User::Display()
                             | Same function name Display() behaves differently
                             | depending on the object type.
                             | Demonstrates runtime polymorphism through
                             | virtual function overriding.
  ---------------------------|--------------------------------------------------
  Method Overriding          | Resource::Display() override
                             | User::Display() override
                             | Derived classes override the pure virtual
                             | Display() function inherited from LibraryEntity.
  ---------------------------|--------------------------------------------------
  Constructor Overloading    | Borrow_Record
                             | One constructor creates a new borrow record.
                             | Another constructor loads saved records
                             | from the file.
                             |
                             | User
                             | One constructor creates a new user.
                             | Another loads existing user data from file.
  ---------------------------|--------------------------------------------------
  Operator Overloading       | Resource::operator==(const Resource&)
                             | Resource::operator==(int)
                             | Allows direct comparison of resources and IDs:
                             | if (resource == id)
  ---------------------------|--------------------------------------------------
  Composition                | Library_Management
                             | Contains ResourceManager, UserManager,
                             | and BorrowManager as member objects.
                             | Demonstrates HAS-A relationship.
  ---------------------------|--------------------------------------------------
  Association                | BorrowManager::borrow_resource()
                             | BorrowManager::returnResource()
                             | Functions interact with User, Resource,
                             | UserManager, and ResourceManager together.
  ---------------------------|--------------------------------------------------
  Data Hiding                | Resource, User, Borrow_Record
                             | Sensitive data like passwords, balances,
                             | and availability are protected using
                             | private access specifiers.
  ---------------------------|--------------------------------------------------
  Modularity                 | Entire Project Structure
                             | Each class handles a separate responsibility.
                             | ResourceManager manages books,
                             | UserManager manages users,
                             | BorrowManager manages borrow records.
  ---------------------------|--------------------------------------------------
  Exception Handling         | ResourceManager::add_Resource()
                             | BorrowManager::borrow_resource()
                             | BorrowManager::returnResource()
                             | Borrow_Record::Fine_calculation()
                             | Library_Management::mainMenu()
                             | try-catch blocks handle invalid input,
                             | system clock errors, and insufficient balance.
  ---------------------------|--------------------------------------------------
  File Handling              | saveToFile(), loadFromFile()
                             | Used in ResourceManager, UserManager,
                             | and BorrowManager using ifstream/ofstream
                             | to permanently store system data.
  ---------------------------|--------------------------------------------------
  Vector (Dynamic Arrays)    | vector<Resource>
                             | vector<User>
                             | vector<Borrow_Record>
                             | Dynamic containers automatically resize
                             | during runtime.
  ---------------------------|--------------------------------------------------
  Input Validation           | Library_Management::mainMenu()
                             | ResourceManager::add_Resource()
                             | BorrowManager::borrow_resource()
                             | cin.fail() and stoi() validation prevent
                             | invalid or non-numeric input.
  ---------------------------|--------------------------------------------------
  Reusability                | Display(), saveToFile(), loadFromFile(),
                             | syncFile()
                             | Functions are written once and reused
                             | throughout the project.
  ---------------------------|--------------------------------------------------
  Menu-Driven Architecture   | mainMenu(), userMenu(), adminMenu()
                             | Structured menus guide all interactions
                             | between users and the system.
  ---------------------------|--------------------------------------------------
  Time-Based Processing      | Borrow_Record::Fine_calculation()
                             | Uses time(), localtime(), strftime(),
                             | and ctime concepts for issue date,
                             | due date, and overdue fine calculation.
  ---------------------------|--------------------------------------------------
  Static Members             | User::Max_borrow_limit (static const int)
                             | User::Default_balance (static constexpr double)
                             | Shared constants used by all User objects.
  ---------------------------|--------------------------------------------------
  STL Algorithms             | transform()
                             | Used in search_by_title() for converting
                             | titles to lowercase during searching.
  ---------------------------|--------------------------------------------------
  System Calls               | system("cls")
                             | system("pause")
                             | system(cmd.c_str())
                             | Used for screen clearing, pausing,
                             | and executing Python email scripts.


================================================================================
--------------------------------------------------------------------------------
4. WHAT WORKS AND WHAT DOES NOT
--------------------------------------------------------------------------------
================================================================================

-------------
  WHAT WORKS:
-------------

  [+] User registration with username, password, name, address, and email.
  [+] Login authentication with password verification.
  [+] Viewing all available library resources.
  [+] Searching resources by title (partial match supported).
  [+] Borrowing a resource (enforces max borrow limit of 2 per user).
  [+] Returning a borrowed resource and updating availability.
  [+] Automatic fine calculation for overdue returns using system time.
  [+] Balance deduction when a fine is applied on return.
  [+] Librarian (Admin) Panel: Add resource, Delete resource, View all
      users, View all borrow records.
  [+] Full file persistence — user, resource, and borrow data saved to
      .txt files and reloaded correctly on the next program run.
  [+] GUI front-end (Python) with a dark-themed interface for registration.
  [+] Confirmation email sent automatically after GUI registration via
      Gmail, with a personalized PDF report attached.
  [+] Input validation across all menus — invalid input does not crash
      the application.
  [+] All user info and balance during registration is sent via email
      with an attached PDF.

------------------------------------------
  WHAT DOES NOT WORK / KNOWN LIMITATIONS:
------------------------------------------

  [-] No password hashing — passwords are stored in plain text in users.txt.
      This is a security concern and would need encryption in production.
  [-] Search only supports title matching. Author or category search is
      not implemented.
  [-] No forgot password or account recovery feature exists.
  [-] The admin password is hardcoded in the source. There is no option
      to change it from within the running program.
  [-] Fine payment is deducted from balance but there is no way to top
      up the balance from inside the program after registration.
  [-] File paths are hardcoded to a specific machine. Running on a
      different computer requires manually updating the paths in source
      code before compiling.

  NOTE: This program is a prototype demonstrating how a real library
        management system could work. The email functionality is also
        based on real-world concepts applied at a prototype level.

================================================================================
--------------------------------------------------------------------------------
5. RESOURCES USED
--------------------------------------------------------------------------------
================================================================================

-----------------------------
  BOOKS / PDFs / WEBSITES:
-----------------------------

  - "C++ How to Program" by Deitel & Deitel
    Used for reference on OOP fundamentals, file I/O, and STL containers.

  - "Object-Oriented Programming in C++" by Robert Lafore
    Referred to for class design, constructors, and encapsulation concepts.

  - "Geek for Geeks library management system"
    (https://www.geeksforgeeks.org/software-engineering/library-management-system/)

  - "NED library system as an idea."

  - "Research gate pdf"
    (https://www.researchgate.net/publication/373137554_Library_Management_System_Mini_Project_Report)

  - FOR GUI: Login page images on Google.

-----------
  AI TOOLS:
-----------

  - Claude by Anthropic (https://claude.ai)
    Used for OOP concept explanations, class design review, report writing,
    and PDF generation.

  - ChatGPT (https://chat.openai.com)
    Used for brainstorming features, C++ syntax help, and logic verification.

  - Trae (https://trae.ai)
    In-editor AI coding assistant used for real-time code completion and
    error detection during development.

  - Gemini (https://gemini.google.com)
    Used for suggesting ideas and alternative approaches.

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
================================================================================
                         END OF README — CIS SHELF PROJECT
================================================================================
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

-------------------------THANK YOU FOR READING OUR DOCUMENT----------------------