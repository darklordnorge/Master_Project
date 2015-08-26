#ifndef _SIMPLE_AGENTS_
#define _SIMPLE_AGENTS_

#include "world_entity.h"
#include <cmath>
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>
#include "occupancy_map.h"
#define EPSILON (double)(exp(-3.0))


class SIMPLE_Agents : public World_Entity {
  
 protected:
  vector <double> pos;
  vector <double> dim;
  vector <double> rot;
  vector <double> long_term;
  vector <double> medium_term;
  double right_out;
  double left_out;  
  bool frozen;
  double rotation;
  double simulation_time_step;

 Occupancy_Map *map;

  static const double wheel_distance      = 0.052;
  static const double half_wheel_distance = 0.026;
  static const double wheel_radius        = 0.0205;
  static const double wheel_width         = 0.002;
  static const double robot_radius        = 0.037;
  static const double robot_height        = 0.045;
  
  static const int robot_slices           = 20;
  static const double IR_range            = 0.04;
  static const double max_ir_reading      = 4095.0;
  static const double camera_ray          = 0.50;
  static const double pos_noise           = 0.01;
  static const double rot_noise           = 0.0087;
  static const double vel_noise           = 0.0;
  static const double max_vel             = 4.5;
  static const double min_vel             = -4.5;
  static const double prob_long_term_change = 0.02;
  static const double prob_medium_term_change = 0.1;

 public:
  static const int num_IR_sensors =8;
  static const int num_camera_rays_per_sectors = 6;
  static const int num_camera_sectors = 3;
  vector <double> start_pos;
  vector <btVector3> to1;//[8];
  vector <btVector3> from1;//[8];
  vector <btVector3> to2;//[num_camera_sectors*num_camera_rays_per_sectors];
  btVector3 from2;
  btVector3 randb_from;
  btVector3 randb_to;
  vector <double> vel;
  btDynamicsWorld* world ;
  //btCompoundShape* body1;
  //btRigidBody* stand1;
  //btRigidBody* stand2;
  btRigidBody* body;
  btRigidBody* right_wheel;
  btRigidBody* left_wheel;
  btHingeConstraint* right_hinge;
  btHingeConstraint* left_hinge;

 //functions added by stefan
 void take_occupancy_reading(vector <double> &ir_reading, double rotation, double x_coord, double z_coord);
 double get_rotation();
 int get_heading();
 void save(void);
 void init_map(void);

 //

  void addRobotPhysics( const vector <double> &_pos, const vector <double> &_rot );
  void get_IR_reading( vector <double> &_reading);
  void get_camera_reading(vector<double> &_reading);
  void add_noise( vector <double> &readings );
  SIMPLE_Agents();
  SIMPLE_Agents(int ind, double simulation_time_step , btDynamicsWorld *world);
  ~SIMPLE_Agents();
  void   set_pos( const vector <double> &_pos );
  void   set_robot_pos_rot( const vector <double> &_pos, const vector <double> &_rot );
  void   set_rot( const vector <double> &_rot );
  void   set_vel( const vector <double> &_vel );

  double get_randb_reading( vector <double> _to_robot_pos, vector <double> &_reading);
  
  const vector <double> get_pos();
  const vector <double> get_rot();
  inline const vector <double> get_dim( void ){ return dim;}
  inline const vector <double> get_colour  ( void ){ return colour;}
  inline void set_colour ( const vector <double> c ) {
    colour[0] = c[0];
    colour[1] = c[1];
    colour[2] = c[2];
  }
  inline const vector <double> get_vel( void ){return vel;}
  inline static const double   get_max_vel( void ) {return max_vel;}
  inline static const double   get_min_vel( void ) {return min_vel;}
  inline static const double   get_radius( void ){return robot_radius;}

  inline const double get_right_out    ( void ) { return right_out;}
  inline const double get_left_out     ( void ) { return left_out;}
  inline void         set_frozen       ( bool flag ) { frozen = flag;}
  inline const bool   is_frozen        ( void ) { return frozen;}

  void   update_pos_rot ( void );
  double pinkq          ( int gen );
#ifdef _GRAPHICS_
  void   render         ( void );
#endif



};

#endif
