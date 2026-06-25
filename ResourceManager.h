#pragma once
#include "Resource.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>
using namespace std;

class ResourceManager
{
private:
    vector<Resource> resource;
    const string filename = "resources.txt";

public:
    void saveToFile();
    void loadFromFile();
    void add_Resource();
    void display_availableResource();
    void search_by_title();
    void delete_Resource();
    void addDefaultResources();

    vector<Resource> &getResources();
    void syncFile();
};