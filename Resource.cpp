#include "Resource.h"

Resource::Resource() : id(0), title(""), author(""), category(""), available(true) {}

Resource::Resource(int i, string t, string a, string c)
    : id(i), title(t), author(a), category(c), available(true) {}

int Resource::getId() const { return id; }
string Resource::getTitle() const { return title; }
string Resource::getAuthor() const { return author; }
string Resource::getCategory() const { return category; }
bool Resource::isAvailable() const { return available; }

void Resource::setAvailability(bool status) { available = status; }

bool Resource::operator==(const Resource &other) const { return this->id == other.id; }
bool Resource::operator==(int otherId) const { return this->id == otherId; }

void Resource::saveToFile(ofstream &out) const
{
    out << id << "|" << title << "|" << author << "|"
        << category << "|" << available << "\n";
}

void Resource::Display() const
{
    cout << id << " | " << title << " | " << author << " | "
         << category << " | AVAILABLE: " << (available ? "YES" : "NO") << endl;
}

void Resource::Display(int color) const
{
    setColor(color);
    Display();
    setColor(15);
}