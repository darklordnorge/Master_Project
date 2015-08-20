//
// Created by stefan on 15/08/15.
//

#include "occupancy_map.h"
#define MODE  "a"
Occupancy_Map::Occupancy_Map(){

}

void Occupancy_Map::init() {
//    map = {0.0}; //initialise the map to 0.0
}

void Occupancy_Map::init_matrix() {
    for(int i = 0;i < map_height;i++){
        for(int j = 0;j < map_width;j++){
            map[i][j] = 0;
        }
    }
}

void Occupancy_Map::update_matrix(int x_pos, int y_pos, int value){
    map[x_pos][y_pos] = value;
}

void Occupancy_Map::calc_robot_pos(double x_coord, double y_coord){
    int matrix_x, matrix_y = 0;
    double integral, fractal ;

    if(x_coord > 0.0){
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

    map[matrix_x][matrix_y] = 1;
}

void Occupancy_Map::calc_matrix_values(vector<double> &ir_reading, double rotation, int robot_x, int robot_y) {
    double sensor_value;
    int heading;

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
