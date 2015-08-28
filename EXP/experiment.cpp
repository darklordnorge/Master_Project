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

    map = new Occupancy_Map(); //init map
    matrix = map->init();


  init_local_variables();

}

/* ---------------------------------------------------------------------------------------- */

EXP_Class::~EXP_Class( ){
    delete param;
    delete map;
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
     agent_interface[r].inputs.assign  (param->nets[r]->get_num_input(), 0.0);
     agent_interface[r].outputs.assign (param->nets[r]->get_num_output(), 0.0);
     partial_fitness[param->num_agents];
//     param->agent[0]->init_map();
 }
//    matrix = map->init();

 this->set_agent_position();
}

/* ---------------------------------------------------------------------------------------- */

void EXP_Class::init_evolutionary_run( void ){
    for(int r = 0;r < param->num_agents;r++){
        generation = 0; //This is the counter for the generation
        param->init_ga(param->nets[r]->get_genotype_length(), 1);
    }
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
      param->nets[r]->init(  param->ga->get_solution( genotype, 0 ) );
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
    for(int r = 0;r < param->num_agents;r++){
        partial_fitness[r] = 0.0;
    }

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
    param->nets[r]->set_genotype_length ( copy_genes.size() );
#ifdef _HETERO_GROUP_SELECTION_
    for(int g = 0; g < copy_genes.size(); g++){
      copy_genes[g] = genes[g+(copy_genes.size()*r)];
    }
    param->nets[r]->init(  copy_genes );max_geno_to_eval
#else
    // you need to uncommet this line after you design you controller
    param->nets[r]->init( genes );
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
    manage_collisions ();
    //if(param->agent[0]->get_pos()[2] > 2.00) iter = param->num_iterations;
    compute_fitness_each_step();
    occupancy_reading();
    iter++;
//    map->start();
//    map.init();
//    int head = param->agent[0]->get_heading();
//    double rot = param->agent[0]->get_rotation();
//    printf("Rotation is: %f , heading is: %d\n", rot, head);
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
    for(int r=0; r < param->num_agents; r++){
          agent_interface[r].outputs.assign(param->nets[0]->get_num_output(),0.0);
    }

  //update robot controllers
  for(int r=0; r < param->num_agents; r++){
        // you need to uncommet this line after you design you controller
        param->nets[r]->step( agent_interface[r].inputs, agent_interface[r].outputs);

    }

}

/* ---------------------------------------------------------------------------------------- */
// this function just set the out of neural controller to robot's wheels velocity variable
void EXP_Class::update_Actuators( void ){
    /*update robot wheels velocity*/
  for(int r=0; r < param->num_agents; r++){
     param->agent[r]->set_vel(agent_interface[r].outputs);
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
    for(int r = 0;r < param->num_agents;r++){
        FINAL_FITNESS[0] = partial_fitness[r] / (double)(param->num_iterations);
    }

}

/*------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------*/
/*fitness = mean(leftSpeed, rightSpeed) * (1 - sqrt(abs(speedLeft) - abs(speedRight))*(1 - highest IR reading )*/
/*-------------------------------------------------------------------------------------------------------------*/
void EXP_Class::compute_fitness_each_step( void ){
    vector <double> randB_reading;
    randB_reading.assign(2, 0.0);
    for(int r=0; r < param->num_agents; r++) {
        double vl = ((param->agent[r]->get_vel()[0] / param->agent[r]->get_max_vel()) + 1) * 0.5;
        double vr = ((param->agent[r]->get_vel()[1] / param->agent[r]->get_max_vel()) + 1) * 0.5;
        double comp_1 = (vl + vr) * 0.5;
        double comp_2 = 1.0 - sqrt(fabs(vl - vr));
        double comp_3 = 0.0;
        for (int i = 0; i < agent_interface[r].inputs.size(); i++) {
            if (comp_3 < agent_interface[r].inputs[i])
                comp_3 = agent_interface[r].inputs[i];
        }

        comp_3 = (1.0 - comp_3);
        double comp_4 = 0.0;
        if (param->num_agents != 1) {
            if (r == param->num_agents-1) {
                param->agent[r]->get_randb_reading(param->agent[r - 1]->get_pos(), randB_reading);
                comp_4 = randB_reading[0];
            }
            else {
                param->agent[r]->get_randb_reading(param->agent[r + 1]->get_pos(), randB_reading);
                comp_4 = randB_reading[0];
            }
        }

        if(comp_4 > 0.6){
            partial_fitness[r] = 0;
        }
        else{
            partial_fitness[r] += comp_1 * comp_2 * comp_3 * comp_4 * param->agent[r]->get_pos()[2];
        }
//        cout << "Range" << comp_4 << endl;

    }
//    partial_fitness += comp_1 *comp_2 *comp_3 * param->agent[0]->get_pos()[2];
//    partial_fitness += comp_1 * comp_2 * comp_3;
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

    compute_fitness();

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
//                map->save_map();
            }
            init_single_evaluation( );
//            param->agent[0]->save();
            map->save_map(matrix);
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

/*Takes the occupnacy reading for each robot in the swarm and updates the global map*/
void EXP_Class::occupancy_reading() {
    int *robot_pos;
    int heading;
    vector<double> reading;


    for(int i = 0;i  < param->num_agents;i++){
        robot_pos = map->calc_robot_pos(param->agent[i]->get_pos()[0], param->agent[i]->get_pos()[2]); //aquire heading and positon info
        heading = map->calc_heading(param->agent[i]->get_rotation());
        reading.assign(param->nets[i]->get_num_input(), 0.0); //resize reading variable

         param->agent[i]->get_IR_reading(reading);     //aquire IR readings

        /*set occupied fields*/
        for(int j = 0;j < reading.size();j++){
            if(reading[j] >= 1000){
                map->calc_matrix_values(agent_interface[i].inputs, heading, robot_pos[0], robot_pos[1], matrix);
            }

        }

        map->mark_cell(robot_pos[0], robot_pos[1], 2, matrix); //set cell as occupied by the robot
    }


}


