#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "LibraryEntity.h"
#include "Utils.h"
using namespace std;

class Resource : public LibraryEntity
{
private:
    int id;
    string title;
    string author;
    string category;
    bool available;

public:
    Resource();
    Resource(int i, string t, string a, string c);

    int getId() const;
    string getTitle() const;
    string getAuthor() const;
    string getCategory() const;
    bool isAvailable() const;

    void setAvailability(bool status);

    bool operator==(const Resource &other) const;
    bool operator==(int otherId) const;

    void saveToFile(ofstream &out) const;

    void Display() const override;
    void Display(int color) const;
};