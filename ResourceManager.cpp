#include "ResourceManager.h"

void ResourceManager::saveToFile()
{
    ofstream out(filename);
    for (auto &r : resource)
        r.saveToFile(out);
}

void ResourceManager::loadFromFile()
{
    ifstream in(filename);
    if (!in) return;

    string line;
    while (getline(in, line))
    {
        if (line.empty()) continue;
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

void ResourceManager::add_Resource()
{
    string idInput, Title, Author, Category;

    cout << "ID: ";
    getline(cin, idInput);

    int id;
    try
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

void ResourceManager::display_availableResource()
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

void ResourceManager::search_by_title()
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

void ResourceManager::delete_Resource()
{
    int id;
    cout << "Enter Resource ID to delete: ";
    cin >> id;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (auto it = resource.begin(); it != resource.end(); ++it)
    {
        if (it->getId() == id)
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

void ResourceManager::addDefaultResources()
{
    if (!resource.empty()) return;

    resource.emplace_back(1, "Harry Potter", "J.K. Rowling", "Fantasy");
    resource.emplace_back(2, "The Alchemist", "Paulo Coelho", "Fiction");
    resource.emplace_back(3, "Clean Code", "Robert C. Martin", "Programming");
    resource.emplace_back(4, "Introduction to Algorithms", "Cormen et al.", "Programming");
    resource.emplace_back(5, "Pride and Prejudice", "Jane Austen", "Classic");
    saveToFile();
}

vector<Resource> &ResourceManager::getResources() { return resource; }
void ResourceManager::syncFile() { saveToFile(); }