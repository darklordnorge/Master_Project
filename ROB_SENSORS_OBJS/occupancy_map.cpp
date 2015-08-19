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
