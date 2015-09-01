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
#include "math.h"
#include "iterator"

using namespace std;
class Occupancy_Map{


private:
    static const int map_width = 2500;
    static const int map_height = 2500;
//    int **map = new int*[2500]; //cool way to do it. Not possible since non static
//       int *map;
//    vector<vector<int> > map;
    int **map;

public:
    Occupancy_Map();
    ~Occupancy_Map();
    int ** init();
    FILE* open_map(const char* filename);
    void update_map(const char* filename, double x_coord, double z_coord);
    void update_matrix(int x_pos, int y_pos, int value);
    int* calc_robot_pos(double x_coord, double y_coord);
    void calc_matrix_values(vector <double> &ir_reading, int heading, int robot_x, int robot_y, int** matrix);
    void set_front_cells(int heading, int sensor, int robot_x, int robot_y, int** matrix);
    void set_front_side_cells(int heading, int sensor, int robot_x, int robot_y, int** matrix);
    void set_side_cells(int heading, int sensor, int robot_x, int robot_y, int** matrix);
    void set_aft_cells(int heading, int sensor, int robot_x, int robot_y, int** matrix);
    void set_angeld_cells(int heading, int sensor, int robot_x, int robot_y, int** matrix);
    void mark_cell(int x_coord, int y_coord, int mark, int** matrix);
    int calc_sensor(int array_num);
    int calc_heading(double rotation);
    void save_map(int** matrix);

};


#endif //PROJECT_OCCUPANCY_MAP_H
