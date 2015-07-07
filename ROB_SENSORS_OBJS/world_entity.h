#ifndef _WORLD_ENTITY_
#define _WORLD_ENTITY_

#include <vector>
#include <map>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

#include "../MISC/general.h"

#ifdef _GRAPHICS_
#include <GL/glut.h>
#endif

#define NOISE_LEVEL 1.0
#define PI     3.141592654
#define TWO_PI 6.283185307

using namespace std;

class World_Entity {
public:
  int collision_counter;
  int* collision_objects[10];
protected:

  int index;
  int type_id;
  bool   crashed;
  vector <double> colour;

public:
  double mass;
  World_Entity(){}
  inline const int   get_index       ( void ) { return index;}
  inline const bool   is_crashed       ( void ) { return crashed;}
  inline const double   get_mass       ( void ) { return mass;}
  inline void         set_crashed      ( bool val ) { crashed = val;}
  inline const int             get_type_id ( void ){ return type_id;}
  virtual ~World_Entity(){}
  static double noise_level;  
 
  virtual void  set_pos                     ( const vector <double> &_pos ) = 0;
  virtual void  set_rot                     ( const vector <double> &_rot ) = 0;
  virtual const vector <double> get_pos     ( void ) = 0;;
  virtual const vector <double> get_dim     ( void ) = 0;
  virtual const vector <double> get_colour  ( void ) = 0;
  virtual const vector <double> get_rot     ( void ) = 0;
  
  /* ------------------------------------------------------------ */
  inline void set_colour( const vector <double> &c ) { 
    colour[0] = c[0];
    colour[1] = c[1];
    colour[2] = c[2];
  }
  /* ------------------------------------------------------------ */
  
};

#endif

