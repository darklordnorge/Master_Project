#ifndef _EXP_
#define _EXP_

#include "../MISC/parameters.h"
#include <math.h>
#ifdef _GRAPHICS_
#include "../EXP/RENDERING/engine.h"

class EXP_Class : public Engine {
#endif
  
#ifndef _GRAPHICS_
  class EXP_Class {
#endif
    
  private:
    /* --------------- FIXED MEMBERS --------------- */
    int  generation;//Count the num. of generation
    int  from_gen;
    int  to_gen;
    int  num_core;
    int  genotype; //Count the num. of genotype (indivituals) population size
    int  eval; //Count the num. of evaluation
    int  num_collision;
    int  iter; //Count the num. of iterations
    bool evolution;
    bool viewing;
    bool re_evaluation;
    vector <double> evaluatoinal_min_fitness;
    double generational_avg_fitness;
    double generational_max_fitness;
    double generational_min_fitness;
    vector <double> FINAL_FITNESS;
    vector <double> centre;
    vector <double> sensors_inputs,motor_outputs;
  
    double AGGREGATE_FITNESS;
    double fitnesses_mult;
    /* --------------------------------------------- */

  vector <double> ir_readings;
  vector <double> camera_sector_readings;

 public:

  /* --------------- FIXED FUNCTIONS --------------- */
  EXP_Class (const char *run_name, bool _evolution, bool _viewing, bool _re_evaluation, unsigned long _seed, int _from_gen, int _to_gen , int _num_cores);
  ~EXP_Class( );
  typedef struct {
                     vector <double> inputs;
                     vector <double> outputs;
                     vector < vector <double> > fitness;
   } Interface;
 vector <Interface> agent_interface;
  Parameters *param;
  double find_distance(const vector <double> &_pos1, const vector <double> &_pos2);
  void init_local_variables( void );
  void init_evolutionary_run( void );
  void init_genotype_loop( );
  void init_evaluations_loop( void );
  void init_single_evaluation( void );
  void set_agent_position( void );
  void from_genome_to_controllers( const char *str_source, const int which_genotype );
  void adv ( void );
  void assign_fitness_to_genotype( void );

  void finalise_single_evaluation( void );
  void finalise_evaluations_loop( void );
  void finalise_genotypes_loop( void );
  void finalise_evolutionary_run( void );
  
  bool stop_iterations_loop( void );
  bool stop_evaluations_loop( void ); 
  bool stop_genotypes_loop( void ); 
  bool stop_generations_loop( void );
  
  void update_sensors( void );
  void update_controllers( void );
  void update_Actuators( void );
  void update_world( void );
  void manage_collisions ( void );
  void compute_fitness ( void );
   void dump_statistics              ( const char *locationOfFileTodump, const char *fileName, int generation );
    /* ----------------------------------------------- */

    //stuff added by stefan
    double partial_fitness;
    void compute_fitness_each_step(void);
};
#endif
