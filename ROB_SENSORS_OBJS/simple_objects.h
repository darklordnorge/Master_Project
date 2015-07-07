#ifndef _SIMPLE_OBJECTS_
#define _SIMPLE_OBJECTS_

#include "world_entity.h"
#include <bullet/btBulletDynamicsCommon.h>
using namespace std;

class SIMPLE_Objects : public World_Entity {
  
 protected:
  vector <double> pos;
  vector <double> rot;
  vector <double> dim;
  double angle;
  btQuaternion rotation;
  //int type_id;
  static const int robot_slices    = 20;
  
 private:
  
 public:
  vector <double> start_pos;
  btDynamicsWorld* world;
  btRigidBody* body;
  SIMPLE_Objects(btDynamicsWorld* world);
  virtual ~SIMPLE_Objects(){}
  void set_mass (double _mass);
  void set_pos ( const vector <double> &_pos );
  void set_rot ( const vector <double> &_rot );
  const vector <double> get_pos     ( void );//{ return pos;}
  const vector <double> get_rot     ( void );//{ return rot;}
  inline const vector <double> get_dim     ( void ){ return dim;}
  inline const vector <double> get_colour  ( void ){ return colour;}
  virtual void reset_pos( void ) = 0;
 // inline const int             get_type_id ( void ){ return type_id;}
  
#ifdef _GRAPHICS_
  virtual void    render                   ( ) = 0;
#endif
  
};

/* ------------------------------------------------------------ */

class SIMPLE_Plane : public SIMPLE_Objects {

 public:
  SIMPLE_Plane(btDynamicsWorld* world );
  void addPlane();
  void reset_pos(){}

#ifdef _GRAPHICS_
  void    render                   ( );
#endif
};

/* ------------------------------------------------------------ */

class SIMPLE_Brick : public SIMPLE_Objects {
 public:
  SIMPLE_Brick( int ind, double *data,  btDynamicsWorld* world );
  void reset_pos();

#ifdef _GRAPHICS_
  void    render                   ( );
#endif
};

/* ------------------------------------------------------------ */

class SIMPLE_Cylinder : public SIMPLE_Objects {
  
 public:
  SIMPLE_Cylinder(int ind, double *data , btDynamicsWorld *world);
  void reset_pos( void );
#ifdef _GRAPHICS_
  void    render                   ( );
#endif
};

/* ------------------------------------------------------------ */

class SIMPLE_Sphere : public SIMPLE_Objects {
  
 public:
  SIMPLE_Sphere( int ind, double *data,  btDynamicsWorld* world);
  void reset_pos( void );
#ifdef _GRAPHICS_
  void    render                     ( );
#endif
};

/* ------------------------------------------------------------ */

class SIMPLE_Light : public SIMPLE_Objects {
  
 public:
  SIMPLE_Light( double *data, btDynamicsWorld* world );
  void reset_pos( void ){}
#ifdef _GRAPHICS_
  void    render                     ( );
#endif
};

/* ------------------------------------------------------------ */

#endif
