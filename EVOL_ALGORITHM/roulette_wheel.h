#ifndef _ROULETTE_WHEEL_
#define _ROULETTE_WHEEL_

#include "ga.h"
#include "chromosome.h"

//#define _RANK_BASED_

using namespace std;

class Roulette_wheel : public GA { 
  
 private:
  vector <double> wheel;
  vector< ValueWithIndex <double> > fitness;

  Chromosome <chromosome_type> **chromosome;
  Chromosome <chromosome_type> **tmp_chromosome;

 public:
  Roulette_wheel ( const int num_alleles_, const int num_bases_per_allele_, const int num_agents_per_group_ );
  ~Roulette_wheel( );

  /* -------------------------------------------------------------------------------------------------- */
  /*                                             VIRTUAL FUNCTIONS                                      */
  /* -------------------------------------------------------------------------------------------------- */  
  void breeding                                  ( void );
  void assign_fitness                            ( const int ind, const vector <double> & final_fitness );
  void dump_genome_into_file                     ( const char *dir, const char *fileName, const int generation );
  void upload_genome_from_file                   ( const char *dir, const char *fileName, const int generation );
  inline vector <chromosome_type> & get_solution ( const int g, const int ind ){ return chromosome[g][ind].allele; }
  /* -------------------------------------------------------------------------------------------------- */
  
  void breeding_group_selection         ( void );
  void compute_cumulative_fitness       ( void );
  void linear_rank_wheel                ( void );
  void select_group                     ( int *mum );
  void select_second_group              ( const int *mum, int *dad );  
  
};

#endif
