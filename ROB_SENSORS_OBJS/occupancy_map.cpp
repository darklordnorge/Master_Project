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
