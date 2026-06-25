#include "UserManager.h"

void UserManager::saveToFile()
{
    ofstream out(filename);
    for (auto &u : users)
        u.saveToFile(out);
}

void UserManager::loadFromFile()
{
    users.clear();

    ifstream in(filename);
    if (!in) return;

    string line;
    while (getline(in, line))
    {
        if (line.empty()) continue;
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

void UserManager::register_user()
{
    string u, p, f, l, a, e;

    cout << "USERNAME: ";  getline(cin, u);
    cout << "PASSWORD: ";  getline(cin, p);
    cout << "FIRST NAME: "; getline(cin, f);
    cout << "LAST NAME: ";  getline(cin, l);
    cout << "ADDRESS: ";   getline(cin, a);
    cout << "EMAIL: ";     getline(cin, e);

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

    string cmd = "cmd /c python send_email.py \"" + su + "\" \"" + sf + "\" \"" + sl + "\" \"" + sa + "\" \"" + se + "\"";
    int result = system(cmd.c_str());
    if (result != 0)
        cout << "Failed to send email notification. Check internet or Python script.\n";
}

string UserManager::login_user()
{
    string u, p;
    cout << "ENTER USERNAME: "; getline(cin, u);
    cout << "ENTER PASSWORD: "; getline(cin, p);

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

User *UserManager::findUser(const string &username)
{
    for (auto &u : users)
        if (u.get_Username() == username)
            return &u;
    return nullptr;
}

void UserManager::printAllUsers()
{
    for (auto &u : users)
        u.Display();
}

vector<User> &UserManager::getUsers() { return users; }
void UserManager::syncFile() { saveToFile(); }