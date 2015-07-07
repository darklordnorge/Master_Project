#include "experiment.h"
#include "../EVOL_ALGORITHM/ga.h"
//#include <stdio.h>
//#include <conio.h>
/* ---------------------------------------------------------------------------------------- */

EXP_Class::EXP_Class(const char *run_name, bool _evolution, bool _viewing, bool _re_evaluation, unsigned long _seed, int _from_gen, int _to_gen, int _num_cores ){
  
  evolution     = _evolution;
  viewing       = _viewing;
  re_evaluation = _re_evaluation;
  from_gen = _from_gen;
  to_gen   = _to_gen;
  num_core = _num_cores;
  param     = new Parameters ( run_name );
  if( evolution ) param->set_random_root_seed( _seed );
  param->init_random_generator( );//GSL

  init_local_variables();
}

/* ---------------------------------------------------------------------------------------- */

EXP_Class::~EXP_Class( ){
  delete param;
}

/* ---------------------------------------------------------------------------------------- */

void EXP_Class::init_local_variables( void ){
  //In this function you have to create your objects like agents, objects,
  //sensors, obstables world characteristics etc.
  eval       = 0; //This is the counter for the evaluation
  iter       = 0; //This is the counter for the iterations
  genotype   = 0; //This is the counter for the solutions of genotypes
  if( evolution ) param->dump_simulation_seed ( );//This is to dump the seed number in EXP/DATA
  
  param->init_controllers( );
  param->init_objects( );
  param->init_agents( );
  centre.assign(3,0.0);
 //init and reset the array to store Infra-red sensor readings
 ir_readings.assign( param->agent[0]->num_IR_sensors, 0.0);
 //init and reset the camera array
 camera_sector_readings.assign( param->agent[0]->num_camera_sectors, 0.0);

 agent_interface.resize( param->num_agents );
 for(int r = 0; r < param->num_agents; r++){
   agent_interface[r].inputs.assign  ( /*param->nets[r]->get_num_input() you need to uncomment this part after you design you controller*/ 8, 0.0);
   agent_interface[r].outputs.assign ( /*param->nets[r]->get_num_output() you need to uncomment this part after you design you controller*/ 4, 0.0);
 }

 this->set_agent_position();
}

/* ---------------------------------------------------------------------------------------- */

void EXP_Class::init_evolutionary_run( void ){

        generation = 0; //This is the counter for the generation
        param->init_ga( /*param->nets[0]->get_genotype_length() you need to uncommet this part after you design you controller*/ 28, 1 );

 
}

/* ---------------------------------------------------------------------------------------- */

void EXP_Class::init_genotype_loop( ){ // this loop for each population
  genotype   = 0;//This is the counter for the population or solutions (genotype)
  generational_avg_fitness     = 0.0;
  generational_max_fitness     = 0.0;
  generational_min_fitness     = 0.0;
 }


/* ---------------------------------------------------------------------------------------- */

// This function is to set the agent starting position and orientation every single evaluation

 void EXP_Class::set_agent_position(){

  vector <double> pos;
  vector <double> rot;

  rot.assign(3,0.0);
  pos.assign(3,0.0);

  param->resetPhysicsState();
  for(int obj=0;obj<param->num_objects;obj++){
      if(param->object[obj]->get_mass() != 0.0){
            param->object[obj]->reset_pos();
            param->object[obj]->body->clearForces();
            param->object[obj]->body->setLinearVelocity(btVector3(0.0,0.0,0.0));
            param->object[obj]->body->setAngularVelocity(btVector3(0.0,0.0,0.0));
      }
   }

 pos[0] = 0.0;
 pos[2] = 0.20;
 rot[1] = -0.48 * PI + gsl_rng_uniform_pos( GSL_randon_generator::r_rand )*PI/2 - (PI/4);
 param->agent[0]->set_robot_pos_rot( pos, rot );
}

/* ---------------------------------------------------------------------------------------- */

void EXP_Class::init_evaluations_loop( ){ // this loop for agents life time for single trail

  //There are various thing you want to do here
  // eg: resettng the fitness to zero (const vector <double> & FINAL_FITNESS)
  //printf(" individual = %d\n",genotype);
  eval = 0;//This is the counter for the  number of evalutions in each trail
  fitnesses_mult = 1;
  FINAL_FITNESS.assign(1,0.0);

  num_collision = 0;
  param->reset_seed( );


  if( evolution ) {
    
    for(int r=0; r < param->num_agents; r++) {
#ifdef _HETERO_GROUP_SELECTION_
      //This is where we create a network from solution num: genotype
      param->nets[r]->init(  param->ga->get_solution( genotype, r ) );
#else
      //This is where we create a network from solution num: genotype
      // you need to uncommet this line after you design you controller
      //param->nets[r]->init(  param->ga->get_solution( genotype, 0 ) );
#endif
    }
  }
}

/* ---------------------------------------------------------------------------------------- */

void EXP_Class::init_single_evaluation( void ){ 
    iter = 0;//This is the counter for the iteration (steps) no. of iteration define the life time of the agent in singe evaluation

    set_agent_position();
    // you need to uncommet this loop if you want to reset controller after you design one
    /*
    for(int r=0; r < param->num_agents; r++) {
          param->nets[r]->reset();
    }
    */
   // printf("\n  eval%d final fitness=%f ",eval,FINAL_FITNESS[0]);

}

/* ---------------------------------------------------------------------------------------- */

void EXP_Class::from_genome_to_controllers( const char *str_source, const int which_genotype ){
  int gen_length = 0;
  vector <chromosome_type> genes;
  vector <chromosome_type> copy_genes;
  
#ifdef _HETERO_GROUP_SELECTION_
  upload_parameters_from_file <chromosome_type> ( str_source, which_genotype, genes, &gen_length, param->num_agents );
  copy_genes.assign( gen_length*param->num_agents, 0.0);
#else
  upload_parameters_from_file <chromosome_type> ( str_source, which_genotype, genes, &gen_length, 1 );
  copy_genes.assign( gen_length, 0.0);
#endif
  
  for(int r=0; r < param->num_agents; r++) {
    // you need to uncommet this line after you design you controller
    //param->nets[r]->set_genotype_length ( copy_genes.size() );
#ifdef _HETERO_GROUP_SELECTION_
    for(int g = 0; g < copy_genes.size(); g++){
      copy_genes[g] = genes[g+(copy_genes.size()*r)];
    }
    param->nets[r]->init(  copy_genes );max_geno_to_eval
#else
    // you need to uncommet this line after you design you controller
    //param->nets[r]->init( genes );
#endif
  }
}

/* ---------------------------------------------------------------------------------------- */

void EXP_Class::adv ( void ){
  if( viewing ) stop_iterations_loop( );
  update_sensors( );
  update_controllers ( );
  update_Actuators();
  for(int i=0;i < 6;i++){
        update_world();
        param->world->stepSimulation( param->physics_step);
  }
  //manage_collisions ();
  //if(param->agent[0]->get_pos()[2] > 2.00) iter = param->num_iterations;
  iter++;
}


/* ---------------------------------------------------------------------------------------- */

void EXP_Class::update_sensors( void ){


    for(int r=0; r < param->num_agents; r++){
      if( !param->agent[r]->is_frozen() ) {
          for (int i = 0; i < ir_readings.size(); i++) ir_readings[i] = -1.0;
         // for (int i = 0; i < param->agent[0]->num_camera_sectors; i++) camera_sector_readings[i] = 0.0;

          // read the infra-red value for every iteration
          param->agent[r]->get_IR_reading(ir_readings);
          param->agent[r]->add_noise(ir_readings);

          for( int i = 0; i < param->agent[r]->num_IR_sensors; i++){
               agent_interface[r].inputs[i] = ir_readings[i];
              // printf("Robot%d ir%d reading= %f\n",r,i,agent_interface[r].inputs[i]);
          }

        // read the camera sector values if case need to use camera
//           param->agent[r]->get_camera_reading(camera_sector_readings);

//          for( int i = 0; i < param->agent[r]->num_camera_sectors; i++){
//                agent_interface[r].inputs[i+8] = camera_sector_readings[i];
//                printf("\n Robot%d camera sector%d reading= %f\n",r,i,agent_interface[r].inputs[i+8]);
//          }

      }

    }
}


/* ---------------------------------------------------------------------------------------- */


void EXP_Class::update_controllers( void ){
    //When you allocate memory for the input and output vector
  //use the following functions:
  //param->nets[r]->get_num_input()
   /* for(int r=0; r < param->num_agents; r++){
          agent_interface[r].outputs.assign(param->nets[0]->get_num_output(),0.0);
    } */

  //update robot controllers
  for(int r=0; r < param->num_agents; r++){
        // you need to uncommet this line after you design you controller
        //param->nets[r]->step( agent_interface[r].inputs /* represent the input from robot's infra-red(s) and or camera */ , agent_interface[r].outputs /* represent the output to set robot wheels speed */ );

    }

}

/* ---------------------------------------------------------------------------------------- */
// this function just set the out of neural controller to robot's wheels velocity variable
void EXP_Class::update_Actuators( void ){
  vector <double> outputs;
  outputs.resize(4);
  outputs[0] = 1.0;
  outputs[1] = 0.0;
  outputs[2] = 1.0;
  outputs[3] = 0.0;


  for(int r=0; r < param->num_agents; r++){
      //update robot wheels velocity
      param->agent[r]->set_vel(outputs);
  }
}

/* ---------------------------------------------------------------------------------------- */


/* ---------------------------------------------------------------------------------------- */
// this function set the actual(i.e.physical) wheels rotational velocity for velocity variable
void EXP_Class::update_world( void ){

  for(int r=0; r < param->num_agents; r++){
      //set the wheels speed in physics engine
      param->agent[r]->update_pos_rot( );
    }
}

/* ---------------------------------------------------------------------------------------- */

// the function is just to capture collision of the robot with another robot or object
void EXP_Class::manage_collisions (void ){

     if(param->agent[0]->is_crashed()){
         param->agent[0]->set_crashed(false);
         iter = param->num_iterations;
     }
}



/* ---------------------------------------------------------------------------------------- */

// This function is what you need to design to guide the evoluation towards the solution
void EXP_Class::compute_fitness( void ){
   double f;
   f = param->agent[0]->get_pos()[2];
   //printf("\n single fitness = %f",f);
   FINAL_FITNESS[0] += f;

}



/* ---------------------------------------------------------------------------------------- */

// function is just to find distance between any two robots or object
// to use this function you need to call get_pos() function for agent or object

double EXP_Class::find_distance(const vector<double> &_pos1, const vector<double> &_pos2 ){

    return sqrt(((_pos2[0]-_pos1[0])*(_pos2[0]-_pos1[0]) + (_pos2[2]-_pos1[2])*(_pos2[2]-_pos1[2])));

}


/* ---------------------------------------------------------------------------------------- */

void EXP_Class::finalise_single_evaluation ( void ){

    //here you can call compute_fitness if you need to evaluate agent at the end of every evaluation

    //compute_fitness();

}

/* ---------------------------------ir_readings------------------------------------------------------- */

// this function get called at the end of each trail

void EXP_Class::finalise_evaluations_loop( void ){


    FINAL_FITNESS[0]= FINAL_FITNESS[0] / ((double)(param->num_evaluations));
    //printf("\n final fitness = %f",FINAL_FITNESS[0]);
  generational_avg_fitness += FINAL_FITNESS[0] /*FINAL_FITNESS*/;
  if( genotype == 0 ){
    generational_max_fitness     = FINAL_FITNESS[0]/*FINAL_FITNESS*/;
    generational_min_fitness     = FINAL_FITNESS[0] /*FINAL_FITNESS*/;
  }
  else{
    if( /*FINAL_FITNESS*/ FINAL_FITNESS[0] > generational_max_fitness ){
      generational_max_fitness     = FINAL_FITNESS[0] /*FINAL_FITNESS*/;
    }
    else if(/*FINAL_FITNESS*/FINAL_FITNESS[0]  < generational_min_fitness )
      generational_min_fitness = FINAL_FITNESS[0]/*FINAL_FITNESS*/;
  }

}

/* ---------------------------------------------------------------------------------------- */

void EXP_Class::assign_fitness_to_genotype( void ){

   param->ga->assign_fitness( genotype,  FINAL_FITNESS );
}

/* ---------------------------------------------------------------------------------------- */

void EXP_Class::finalise_genotypes_loop( void ){
        if( !(generation%param->dump_stats_every) )
            dump_statistics( param->statsFileDir, param->statsFileName, generation);
        if( !(generation%param->dump_genome_every) )
            param->ga->dump_genome_into_file( param->genomeFileDir,  param->genomeFileName, generation );

        //This is the function that create s a new generation of solutions (genotypes)
        param->ga->breeding();

}

/* ---------------------------------------------------------------------------------------- */

void EXP_Class::finalise_evolutionary_run( void ){

}

/* ---------------------------------------------------------------------------------------- */

//This function should not be changed
bool EXP_Class::stop_iterations_loop( void ){

    if(iter >= param->num_iterations){
        finalise_single_evaluation( );
        iter = 0;
        if( viewing ) stop_evaluations_loop( );
        return false;
    }
    else
        return true;
}

/* ---------------------------------------------------------------------------------------- */

bool EXP_Class::stop_evaluations_loop( void ){
  
    eval++;
    if( evolution ){
        if( eval >= param->num_evaluations ) {
            eval = 0;
            finalise_evaluations_loop( );
            assign_fitness_to_genotype();
            return false;
        }
        else{
            return true;
        }
    }
    else{
        if( viewing ) {
            if( eval >= param->num_evaluations ) {
                eval = 0;
                finalise_evaluations_loop( );
            }
            init_single_evaluation( );
            return true;
        }

      else if( re_evaluation ){
      if( eval >= param->num_evaluations ) {
      eval = 0;
      finalise_evaluations_loop( );
      return false;
      }
      else{
      return true;
      }
      }

  }
}

/* ---------------------------------------------------------------------------------------- */

bool EXP_Class::stop_genotypes_loop( void ){
    if(evolution){
  genotype++;
  if( genotype >= param->ga->get_num_tot_solutions() ) {
    finalise_genotypes_loop( );
    genotype = 0;
    return false;
  }
  else
    return true;
    }
    else if(re_evaluation){

        genotype = 0;
        return false;
    }
}

/* ---------------------------------------------------------------------------------------- */

bool EXP_Class::stop_generations_loop( void ){
    generation++;
    if(evolution){
        if( generation >= param->num_generations ) {
            finalise_evolutionary_run();
            generation = 0;
            return false;
        }
        else
            return true;
    }
    else if(re_evaluation){
        if( generation > to_gen ){
            return false;
        }
        else{
            return true;
        }

    }
}

/* ---------------------------------------------------------------------------------------- */

void EXP_Class::dump_statistics( const char *locationOfFileTodump, 
				      const char *fileName, int generation ) {
  char fname_statistics[500];
  sprintf(fname_statistics, "%s%s.data", locationOfFileTodump, fileName ); 
  ofstream outfile;
  outfile.open (fname_statistics, ios::app);
  outfile.setf(ios::fixed);
  
  outfile << " " << generation
   	  << " " << generational_max_fitness                                       //the fitness of the best
   	  << " " << (generational_avg_fitness)/(double)(param->ga->get_num_tot_solutions() )  // the mean fitness
	  << " " << generational_min_fitness                                       //the fitness of the worst
      	  << endl;
  outfile.close();
}

/* ---------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------- */




