//
// Created by stefan on 15/08/15.
//

#include "occupancy_map.h"
#define MODE  "a"


Occupancy_Map::Occupancy_Map(){

}

Occupancy_Map::~Occupancy_Map() {
    for(int i = 0;i < map_height;i++){
        delete[] map[i];
    }
    delete[] map;
}

int** Occupancy_Map::init() {
//    map.resize(2500, vector<int>(2500, 0)); //initilaize and resize the vector. Fill the vector with 0
//    for(int i = 0;i < map_height;i++){
//        vector <int> row;                   //create empty vector
//        row.assign(map_width, 0);           // assign length and set all elements to 0
//        map.push_back(row);                 //push to the main vector
//    }

//    map = new int[map_height][map_width];



    map = new int *[map_height];       //initialise array of pointers
    for(int i = 0;i < map_height;i++){ //add array pointers to the array
        map[i] = new int[map_width];
    }

    for(int i = 0;i < map_height;i++){
        for(int j = 0;j < map_width;j++){
            map[i][j] = 0;
        }
    }

    return map;
//

//    printf("Vector size: %lu", map.size());
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

int Occupancy_Map::calc_sensor(int array_num) {
    int sensor_num;

    if(array_num == 0){
        sensor_num = 0;
    }
    else if(array_num == 1){
        sensor_num == 7;
    }
    else if(array_num == 2){
        sensor_num == 1;
    }
    else if(array_num == 3){
        sensor_num == 6;
    }
    else if(array_num == 4){
        sensor_num == 2;
    }
    else if(array_num == 5){
        sensor_num == 5;
    }
    else if(array_num == 6){
        sensor_num == 3;
    }
    else if(array_num == 7){
        sensor_num == 4;
    }

    return sensor_num;
}

void Occupancy_Map::calc_matrix_values(vector <double> &ir_reading, int heading, int robot_x, int robot_y, int **matrix){
    double sensor_value;
    int sensor_num;
    for(int i = 0;i < ir_reading.size();i++){
        sensor_value = ir_reading[i];
        if(sensor_value != -1){
            sensor_num = calc_sensor(i);
            if(heading == 0 || heading == 1 || heading == 2 || heading == 3){
                if(i == 0 || i == 7){
                    set_front_cells(heading, sensor_num, robot_x, robot_y, matrix);
                }
                else if(i == 6 || i == 1){
                    set_front_side_cells(heading, sensor_num, robot_x, robot_y, matrix);
                }
                else if(i == 5 || i == 2){
                    set_side_cells(heading, sensor_num, robot_x, robot_y, matrix);
                }
                else if(i == 3 || i == 4){
                    set_aft_cells(heading, sensor_num, robot_x, robot_y, matrix);
                }
            }else if(heading == 4 || heading == 5 || heading == 6 || heading == 7){
                set_angeld_cells(heading, sensor_num, robot_x, robot_y, matrix);
            }
        }
    }
}

/*Sensors set on a 15 degree angle to the front of the robot. Sensor 7 and 0 on the robot*/
void Occupancy_Map::set_front_cells(int heading, int sensor, int robot_x, int robot_y, int **matrix) {
    if(heading == 0){
        mark_cell(robot_x, robot_y-1, 1, matrix);
    }
    else if(heading == 1){
        mark_cell(robot_x+1, robot_y, 1, matrix);
    }
    else if(heading == 2){
        mark_cell(robot_x, robot_y+1, 1, matrix);
    }
    else if(heading == 3){
        mark_cell(robot_x-1, robot_y, 1, matrix);
    }
}

/*Sensors placed in a 45 degree angle on the front of the robot. sensor 1 and 6 on the robot*/
void Occupancy_Map::set_front_side_cells(int heading, int sensor, int robot_x, int robot_y, int **matrix) {
    if(sensor == 1){
        if(heading == 0){
            mark_cell(robot_x+1, robot_y-1, 1, matrix);
        }
        else if(heading == 1){
            mark_cell(robot_x+1, robot_y+1, 1, matrix);
        }
        else if(heading == 2){
            mark_cell(robot_x-1, robot_y+1, 1, matrix);
        }
        else if(heading == 3){
            mark_cell(robot_x-1, robot_y-1, 1, matrix);
        }
    }
    else if(sensor == 6){
        if(heading == 0){
            mark_cell(robot_x-1, robot_y-1, 1, matrix);
        }
        else if(heading == 1){
            mark_cell(robot_x+1, robot_y-1, 1, matrix);
        }
        else if(heading == 2){
            mark_cell(robot_x+1, robot_y+1, 1, matrix);
        }
        else if(heading == 3){
            mark_cell(robot_x-1, robot_y+1, 1, matrix);
        }
    }
}
/*Side sensors placed at a 90 degree angle. 2 and 5 on the epuck  */
void Occupancy_Map:: set_side_cells(int heading, int sensor, int robot_x, int robot_y, int **matrix) {
      if(sensor == 2){
         if(heading == 0){
            mark_cell(robot_x+1, robot_y, 1, matrix);
        }
        else if(heading == 1){
            mark_cell(robot_x, robot_y+1, 1, matrix);
        }
        else if(heading == 2){
            mark_cell(robot_x-1, robot_y, 1, matrix);
        }
        else if(heading == 3){
            mark_cell(robot_x, robot_y-1, 1, matrix);
        }
    }
    else if(sensor == 5){
        if(heading == 0){
            mark_cell(robot_x-1, robot_y, 1, matrix);
        }
        else if(heading == 1){
            mark_cell(robot_x, robot_y-1, 1, matrix);
        }
        else if(heading == 2){
            mark_cell(robot_x+1, robot_y, 1, matrix);
        }
        else if(heading == 3){
            mark_cell(robot_x, robot_y+1, 1, matrix);
        }
    }
}

/*Sensors placed at a 25 degree angle to the back of the robot. Sensors 3 and 4 on the epuck*/
void Occupancy_Map::set_aft_cells(int heading, int sensor, int robot_x, int robot_y, int **matrix) {
    if(heading == 0){
        mark_cell(robot_x, robot_y+1, 1, matrix);
    }
    else if(heading == 1){
        mark_cell(robot_x-1, robot_y, 1, matrix);
    }
    else if(heading == 2){
        mark_cell(robot_x, robot_y-1, 1, matrix);
    }
    else if(heading == 3){
        mark_cell(robot_x+1, robot_y, 1, matrix);
    }
}
/*4:northeast, 5:southeast, 6:southwest, 7:northwest*/
void Occupancy_Map::set_angeld_cells(int heading, int sensor, int robot_x, int robot_y, int **matrix) {
    if(heading == 4){
        if(sensor == 7 || sensor == 0){
            mark_cell(robot_x-1, robot_y-1, 1, matrix);
        }
        else if(sensor == 1){
            mark_cell(robot_x+1, robot_y, 1, matrix);
        }
        else if(sensor == 2){
            mark_cell(robot_x+1, robot_y+1, 1, matrix);
        }
        else if(sensor == 4 || sensor == 3){
            mark_cell(robot_x-1, robot_y+1, 1, matrix);
        }
        else if(sensor == 5){
            mark_cell(robot_x-1, robot_y-1, 1, matrix);
        }
        else if(sensor == 6){
            mark_cell(robot_x, robot_y-1, 1, matrix);
        }
    }
    else if(heading == 5){
        if(sensor == 7 || sensor == 0){
            mark_cell(robot_x+1, robot_y+1, 1, matrix);
        }
        else if(sensor == 1){
            mark_cell(robot_x, robot_y+1, 1, matrix);
        }
        else if(sensor == 2){
            mark_cell(robot_x-1, robot_y+1, 1, matrix);
        }
        else if(sensor == 3 || sensor ==4){
            mark_cell(robot_x-1, robot_y-1, 1, matrix);
        }
        else if(sensor == 5){
            mark_cell(robot_x+1, robot_y-1, 1, matrix);
        }
        else if(sensor == 6) {
            mark_cell(robot_x + 1, robot_y, 1, matrix);
        }
    }
    else if(heading == 6){
        if(sensor == 7 || sensor == 0){
            mark_cell(robot_x-1, robot_y+1, 1, matrix);
        }
        else if(sensor == 1){
            mark_cell(robot_x-1, robot_y, 1, matrix);
        }
        else if(sensor == 2){
            mark_cell(robot_x-1, robot_y-1, 1, matrix);
        }
        else if(sensor == 3 || sensor == 4){
            mark_cell(robot_x+1, robot_y-1, 1, matrix);
        }
        else if(sensor == 5){
            mark_cell(robot_x+1, robot_y+1, 1, matrix);
        }
        else if(sensor == 6){
            mark_cell(robot_x, robot_y+1, 1, matrix);
        }
    }
    else if(heading == 7){
        if(sensor == 7 || sensor == 0){
            mark_cell(robot_x-1, robot_y-1, 1, matrix);
        }
        else if(sensor == 1){
            mark_cell(robot_x, robot_y-1, 1, matrix);
        }
        else if(sensor == 2){
            mark_cell(robot_x+1, robot_y-1, 1, matrix);
        }
        else if(sensor == 3 || sensor == 4){
            mark_cell(robot_x+1, robot_y+1, 1, matrix);
        }
        else if(sensor == 5){
            mark_cell(robot_x-1, robot_y+1, 1, matrix);
        }
        else if(sensor == 6){
            mark_cell(robot_x-1, robot_y, 1, matrix);
        }
    }
}

void Occupancy_Map::mark_cell(int x_coord, int y_coord, int mark, int** matrix) {

      matrix[x_coord][y_coord] = mark;

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

void Occupancy_Map::save_map(int** matrix) {
//    ofstream out;
//
//    out.open("map.txt", ios::out);
//    for(int i = 0;i < map_width;i++){
//        for(int j = 0;j < map_height;j++){
//            int x = map[i][j];
//           out.write((char *)x, sizeof x);
//        }
//        out << endl;
//    }
//    out.close();

    FILE *fileptr = fopen("map2.txt", "w");

    if(fileptr == NULL){
        perror("File is NULL");
    }

//    printf("%d", map[5][5]);
    for(int i = 0;i < map_height;i++){
        for(int j = 0;j < map_width;j++){
            if(matrix[i][j] == 1){
                fprintf(fileptr, "%d %d\n", i, j);
            }
//            fprintf(fileptr, "%d,", matrix[i][j]);
        }
//        fprintf(fileptr, "\n");
    }
    fclose(fileptr);

//    ofstream output("map1.txt");
//    ostream_iterator<int> output_iterator(output,"");
//    for(int i = 0;i < 2500;i++){
//        copy(map.at(i).begin(), map.at(i).end(), output_iterator);
//    }
}

