//
// Created by stefan on 15/08/15.
//

#ifndef PROJECT_OCCUPANCY_MAP_H
#define PROJECT_OCCUPANCY_MAP_H

#include "stdio.h"
#include "iostream"
#include "fstream"
#include "vector"
#include "string"
#include "map"
#include "matrix.h"
class Occupancy_Map{


private:
    static const int map_width = 1000;
    static const int map_height = 1000;
    static double map [map_width][map_height];

public:
    Occupancy_Map();
    void init_matrix(void);
    void init (void);
    FILE* open_map(const char* filename);
    void update_map(const char* filename, double x_coord, double z_coord);
};


#endif //PROJECT_OCCUPANCY_MAP_H
