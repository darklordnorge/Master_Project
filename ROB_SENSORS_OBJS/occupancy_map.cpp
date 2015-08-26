//
// Created by stefan on 15/08/15.
//

#include "occupancy_map.h"
#define MODE  "a"
Occupancy_Map::Occupancy_Map(){
    init();
}

Occupancy_Map::~Occupancy_Map() {

}

void Occupancy_Map::init() {
    for(int i = 0;i < map_height;i++){
        for(int j = 0;j < map_width;j++){
            map[i][j] = 0;
        }
    }
}

void Occupancy_Map::update_matrix(int x_pos, int y_pos, int value){
    map[x_pos][y_pos] = value;
}

int* Occupancy_Map::calc_robot_pos(double x_coord, double y_coord){
    int matrix_x = 0;
    int matrix_y = 0;
    int *array = new int[2];
//    int array[2];
    double integral, fractal ;

    if(x_coord > 0.0 ){
        x_coord = x_coord * 1000;
        fractal = modf(x_coord, &integral);
        if(fractal < 0.5){
            x_coord = integral;
        }
        else if(fractal > 0.5){
            x_coord = integral + 1;
        }
        matrix_x = (map_width/2) + x_coord;
        matrix_y = y_coord * 1000;

    }else if(x_coord < 0.0){
        x_coord -= x_coord*2;
        x_coord = x_coord * 1000;

        fractal = modf(x_coord, &integral);
        if(fractal < 0.5){
            x_coord = integral;
        }
        else if(fractal > 0.5){
            x_coord = integral + 1;
        }

        matrix_x = (map_width/2) - x_coord;
        matrix_y = y_coord * 1000;
    }

//    if(matrix_x <= 0){
//        matrix_x = 0; //boundary checking
//    }
//    if(matrix_y <= 0){
//        matrix_y = 0;
//    }

//    map[matrix_x][matrix_y] = 2;
    array[0] = matrix_x;
    array[1] = matrix_y;

    return array;
}

void Occupancy_Map::calc_matrix_values(vector<double> &ir_reading, double rotation, int robot_x, int robot_y) {
    double sensor_value;
    int heading; //set heading to be == the current rotation
    heading = calc_heading(rotation);
    for(int i = 0;i < ir_reading.size();i++){
        sensor_value = ir_reading[i];
        if(sensor_value != -1){
            if(i == 0 || i == 7){
                set_front_cells(heading, i, robot_x, robot_y);
            }
            else if(i == 6 || i == 1){
                set_front_side_cells(heading, i, robot_x, robot_y);
            }
            else if(i == 5 || i == 2){
                set_side_cells(heading, i, robot_x, robot_y);
            }
            else if(i == 3 || i == 4){
                set_aft_cells(heading, i, robot_x, robot_y);
            }
        }
    }
}

/*Sensors set on a 15 degree angle to the front of the robot. Sensor 7 and 0 on the robot*/
void Occupancy_Map::set_front_cells(int heading, int sensor, int robot_x, int robot_y) {
    if(heading == 0){
        mark_cell(robot_x, robot_y-1, 1);
    }
    else if(heading == 1){
        mark_cell(robot_x+1, robot_y, 1);
    }
    else if(heading == 2){
        mark_cell(robot_x, robot_y+1, 1);
    }
    else if(heading == 3){
        mark_cell(robot_x-1, robot_y, 1);
    }
}

/*Sensors placed in a 45 degree angle on the front of the robot. sensor 1 and 6 on the robot*/
void Occupancy_Map::set_front_side_cells(int heading, int sensor, int robot_x, int robot_y) {
    if(sensor == 1){
        if(heading == 0){
            mark_cell(robot_x+1, robot_y-1, 1);
        }
        else if(heading == 1){
            mark_cell(robot_x+1, robot_y+1, 1);
        }
        else if(heading == 2){
            mark_cell(robot_x-1, robot_y+1, 1);
        }
        else if(heading == 3){
            mark_cell(robot_x-1, robot_y-1, 1);
        }
    }
    else if(sensor == 6){
        if(heading == 0){
            mark_cell(robot_x-1, robot_y-1, 1);
        }
        else if(heading == 1){
            mark_cell(robot_x+1, robot_y-1, 1);
        }
        else if(heading == 2){
            mark_cell(robot_x+1, robot_y+1, 1);
        }
        else if(heading == 3){
            mark_cell(robot_x-1, robot_y+1, 1);
        }
    }
}
/*Side sensors placed at a 90 degree angle. 2 and 5 on the epuck  */
void Occupancy_Map::set_side_cells(int heading, int sensor, int robot_x, int robot_y) {
    if(sensor == 2){
        if(heading == 0){
            mark_cell(robot_x+1, robot_y, 1);
        }
        else if(heading == 1){
            mark_cell(robot_x, robot_y+1, 1);
        }
        else if(heading == 2){
            mark_cell(robot_x-1, robot_y, 1);
        }
        else if(heading == 3){
            mark_cell(robot_x, robot_y-1, 1);
        }
    }
    else if(sensor == 5){
        if(heading == 0){
            mark_cell(robot_x-1, robot_y, 1);
        }
        else if(heading == 1){
            mark_cell(robot_x, robot_y-1, 1);
        }
        else if(heading == 2){
            mark_cell(robot_x+1, robot_y, 1);
        }
        else if(heading == 3){
            mark_cell(robot_x, robot_y+1, 1);
        }
    }
}

/*Sensors placed at a 25 degree angle to the back of the robot. Sensors 3 and 4 on the epuck*/
void Occupancy_Map::set_aft_cells(int heading, int sensor, int robot_x, int robot_y) {
    if(heading == 0){
        mark_cell(robot_x, robot_y+1, 1);
    }
    else if(heading == 1){
        mark_cell(robot_x-1, robot_y, 1);
    }
    else if(heading == 2){
        mark_cell(robot_x, robot_y-1, 1);
    }
    else if(heading == 3){
        mark_cell(robot_x+1, robot_y, 1);
    }
}
/*4:northeast, 5:southeast, 6:southwest, 7:northwest*/
void Occupancy_Map::set_angeld_cells(int heading, int sensor, int robot_x, int robot_y) {
    if(heading == 4){
        if(sensor == 7 || sensor == 0){
            mark_cell(robot_x-1, robot_y-1, 1);
        }
        else if(sensor == 1){
            mark_cell(robot_x+1, robot_y, 1);
        }
        else if(sensor == 2){
            mark_cell(robot_x+1, robot_y+1, 1);
        }
        else if(sensor == 4 || sensor == 3){
            mark_cell(robot_x-1, robot_y+1, 1);
        }
        else if(sensor == 5){
            mark_cell(robot_x-1, robot_y-1, 1);
        }
        else if(sensor == 6){
            mark_cell(robot_x, robot_y-1, 1);
        }
    }
    else if(heading == 5){
        if(sensor == 7 || sensor == 0){
            mark_cell(robot_x+1, robot_y+1, 1);
        }
        else if(sensor == 1){
            mark_cell(robot_x, robot_y+1, 1);
        }
        else if(sensor == 2){
            mark_cell(robot_x-1, robot_y+1, 1);
        }
        else if(sensor == 3 || sensor ==4){
            mark_cell(robot_x-1, robot_y-1, 1);
        }
        else if(sensor == 5){
            mark_cell(robot_x+1, robot_y-1, 1);
        }
        else if(sensor == 6) {
            mark_cell(robot_x + 1, robot_y, 1);
        }
    }
    else if(heading == 6){
        if(sensor == 7 || sensor == 0){
            mark_cell(robot_x-1, robot_y+1, 1);
        }
        else if(sensor == 1){
            mark_cell(robot_x-1, robot_y, 1);
        }
        else if(sensor == 2){
            mark_cell(robot_x-1, robot_y-1, 1);
        }
        else if(sensor == 3 || sensor == 4){
            mark_cell(robot_x+1, robot_y-1, 1);
        }
        else if(sensor == 5){
            mark_cell(robot_x+1, robot_y+1, 1);
        }
        else if(sensor == 6){
            mark_cell(robot_x, robot_y+1, 1);
        }
    }
    else if(heading == 7){
        if(sensor == 7 || sensor == 0){
            mark_cell(robot_x-1, robot_y-1, 1);
        }
        else if(sensor == 1){
            mark_cell(robot_x, robot_y-1, 1);
        }
        else if(sensor == 2){
            mark_cell(robot_x+1, robot_y-1, 1);
        }
        else if(sensor == 3 || sensor == 4){
            mark_cell(robot_x+1, robot_y+1, 1);
        }
        else if(sensor == 5){
            mark_cell(robot_x-1, robot_y+1, 1);
        }
        else if(sensor == 6){
            mark_cell(robot_x-1, robot_y, 1);
        }
    }
}

void Occupancy_Map::mark_cell(int x_coord, int y_coord, int mark) {
    map[x_coord][y_coord] = mark;
}


FILE* Occupancy_Map::open_map(const char* filename){
    FILE* p_map = fopen(filename, MODE);

    if(p_map == NULL){
        perror(" Error");
    }
  //  printf("File opened\n");
    return  p_map;
}

void Occupancy_Map::update_map(const char* filename, double x_coord, double z_coord){
    FILE* p_map = open_map(filename);
    fprintf(p_map, "%f %f\n", x_coord, z_coord);
    fclose(p_map);
}

/*  0: north, 1:east, 2:south, 3:west, 4:northeast, 5:southeast, 6:southwest, 7:northwest
 * +/- 10 spread for main headings(north, south, east, west)
 * +/- 5 spread for secondary headings(northeast, southeast, etc)*/

int Occupancy_Map::calc_heading(double rotation) {
    int heading;
    if(rotation <= 10 && rotation >= -10){
        heading = 1;
    }
    else if(rotation <= 100 && rotation >= 80){
        heading = 0;
    }
    else if(rotation >= -170 && rotation >= 170){
        heading = 3;
    }
    else if(rotation <= -80 && rotation >= -100){
        heading = 2;
    }
    else if(rotation <= 50 && rotation >= 40){
        heading = 4;
    }
    else if(rotation <= -40 && rotation >= -50){
        heading = 5;
    }
    else if(rotation <= - 130 && rotation >= -140){
        heading = 6;
    }
    else if(rotation <= 140 && rotation >= 130){
        heading = 7;
    }
    else{
        heading  = -1;
    }
    return heading;
}

void Occupancy_Map::save_map() {
    ofstream out;
    out.open("map.txt");
    for(int i = 0;i < map_width;i++){
        for(int j = 0;j < map_height;j++){
            out << map[i][j];
        }
        out << endl;
    }
    out.close();

}

