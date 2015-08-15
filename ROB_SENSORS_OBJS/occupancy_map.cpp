//
// Created by stefan on 15/08/15.
//

#include "occupancy_map.h"

Occupancy_Map::Occupancy_Map(){

}

FILE* Occupancy_Map::open_map(const char* filename){
    FILE* p_map = fopen(filename, mode);

    if(p_map == NULL){
        perror(" Error");
    }
    printf("File opened");
    return  p_map;
}

void Occupancy_Map::update_map(const char* filename, std::vector <double> coordinates){
    FILE* p_map = open_map(filename);
    fprintf(p_map, "%s %s %s\n", coordinates[0], coordinates[1], coordinates[2]);
    fclose(p_map);
}
