#ifndef _GA_
#define _GA_

#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <map>
#include <algorithm>
#include <vector>
#include "../MISC/general.h"

#ifdef _DIPLOID_
#define DIPLOID_FLAG true
#else
#define DIPLOID_FLAG false
#endif


using namespace std;

template<class T>
struct ValueWithIndex
{
  T value;
  int index;
};
template<class T>
bool operator < (const ValueWithIndex<T>& v1, const ValueWithIndex<T>& v2)
{
  return v1.value < v2.value;
}
template<class T> ValueWithIndex<T>
MakeValueWithIndex(const T& value, int index)
{
  ValueWithIndex<T> ret;
  ret.value = value;
  ret.index = index;
  return ret; 
}

class GA { 
  
 protected:
  int      num_groups;
  int      num_agents_per_group;
  int      num_tot_solutions;
  int      num_solutions_truncated; // no. of solutions would be ignore within a population 
  int      num_elite;
  double   prob_mutation;           
  double   prob_cross_over;
 
 public:
  GA ( );
  GA ( const int num_agents_per_group_ ){
    //Initialise EA parameters
    read_from_file( );
    
    num_agents_per_group = 1;
    num_tot_solutions    = num_groups;
    
#ifdef _HETERO_GROUP_SELECTION_
    num_agents_per_group = num_agents_per_group_;
#ifdef _HETERO_INDIVIDUAL_SELECTION_
    num_tot_solutions    = num_groups * num_agents_per_group;
#endif
#endif
    
  };
  virtual ~GA( ){};
  
  virtual void assign_fitness                     ( const int  ind, const vector <double> & final_fitness ) = 0;
  virtual void breeding                           ( void ) = 0;
  virtual void dump_genome_into_file              ( const char *dir, const char *fileName, const int generation ) = 0;
  virtual void upload_genome_from_file            ( const char *dir, const char *fileName, const int generation ) = 0;
  virtual vector <chromosome_type> & get_solution ( const int group_id, const int ind_id ) = 0;
  
  inline void read_from_file                      ( void ){
    ifstream I ("../EVOL_ALGORITHM/initialisation_param.txt");
    if(!I) {
      cerr << "In ../EVOL_ALGORITHM/ga.cpp file --- File initialisation_param.txt in ../EVOLUTIONARY ALGORITHM not found" <<endl;
      exit(0);
    }
    
    num_groups                    = getInt('=', I);  // population size 
    num_solutions_truncated       = getInt('=', I);
    num_elite                     = getInt('=', I);
    prob_mutation                 = getDouble('=', I); 
    prob_cross_over               = getDouble('=', I);
    
    int check = getInt('=',I);
    I.close();
    if( check != 999 ){
      cerr << " In ../EVOL_ALGORITHM/ initialisation_param.txt --- End check was not 999" << endl;
      exit(0);
    }
  }
  
  inline int get_num_tot_solutions                ( void ){return num_tot_solutions;}
  inline int get_num_groups                       ( void ){return num_groups;}
  
};

#endif
