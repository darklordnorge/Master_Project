//
// Created by stefan on 15/08/15.
//

#ifndef PROJECT_OCCUPANCY_MAP_H
#define PROJECT_OCCUPANCY_MAP_H

#include "stdio.h"
#include "iostream"
#include "fstream"
#include "vector"
class Occupancy_Map{
using namespace std;

private:

public:
    Occupancy_Map();
    FILE* open_map(const std::string& filename);
    void update_map(const string& filename, vector <double> coordinates);
};


#endif //PROJECT_OCCUPANCY_MAP_H
