#ifndef _PARAMETERS_
#define _PARAMETERS_

#include "general.h"
#include <bullet/btBulletDynamicsCommon.h>
#include "../EVOL_ALGORITHM/roulette_wheel.h"
#include "../CONTROLLERS/controller.h"
#include "../ROB_SENSORS_OBJS/world_entity.h"
#include "../ROB_SENSORS_OBJS/simple_agents.h"
#include "../ROB_SENSORS_OBJS/simple_objects.h"
#include "../CONTROLLERS/myController.h"



class Parameters {
  
 private:
  
 public:
    int count;
    int **collision_peer;
    int num_collision_peer;
    double m_Time;
    btClock m_clock;
    btCollisionConfiguration* collisionConfig;
    btDispatcher* dispatcher;
    btBroadphaseInterface*  broadphase;
    btConstraintSolver* solver;
    btDynamicsWorld* world;
  Parameters (const char *run_name );
  ~Parameters( );
  void init_physics_param( void );
  void resetPhysicsState();
  btScalar getDeltaTimeMicroseconds();
  void setDeltaTime(btScalar deltaTime);
  void myProcessCallback(btScalar timeStep);
  //bool callbackFunc(btManifoldPoint& cp, const btCollisionObjectWrapper* obj1, int id1, int index1, const btCollisionObjectWrapper* obj2, int id2, int index2);
  void read_run_parameter_file( const char *run_name );
  void init_random_generator( void );
  void init_controllers( void );
  void init_ga( const int num_allele_per_solution, const int num_bases_per_allele );
  
  void init_objects( void );
  void init_agents( void );
  
  void dump_simulation_seed( void );
  void reset_seed( void );
  inline void set_random_root_seed( unsigned long _seed ){ root_seed = _seed; }
  inline int get_random_root_seed( void ){ return root_seed; }
  inline char* get_run_name( void ){return run_name;}

  char *run_name;
  
  GA *ga;
  MyController **nets;
  SIMPLE_Agents **agent;
  SIMPLE_Objects **object;
  unsigned long root_seed;
  
  int     num_generations;
  int     num_evaluations;
  int     num_iterations;
  int     num_agents;
  int     dump_stats_every;
  int     dump_genome_every;
  
  char    statsFileDir[400];
  char    statsFileName[400];
  char    revalFileDir[400];
  char    revalFileName[400];
  char    genomeFileDir[400];
  char    genomeFileName[400];
  char    rootSeedFileDir[400];
  char    rootSeedFileName[400];
  char    genomeFileName_for_reval[400];
  char    type_of_ga[400];
  char    type_of_controller[400];
  
  double simulation_time_step;
  double physics_step;
  
  int num_bricks;
  int num_cylinders;
  int num_spheres;
  int num_objects;
  int num_lights;
  double** objects_data;
  SIMPLE_Objects  *plane;
};
#endif
