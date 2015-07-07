#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#include "../MISC/general.h"

using namespace std;

/* ------------------------------------------------------------------------------------------ */

/* this function is loading the genotype for the specifed source file (ie. run1_pop_G0.geno) from GENOME folder
   and take which_genotype integer from simulator GUI
*/
template <class T>
void upload_parameters_from_file( const char *str_source, const int which_genotype, vector <T> & genes, int *genotype_length, const int num_agents ){
  genes.clear();
  char genome_to_load[500];
  
  sprintf(genome_to_load, "%s", str_source );
  string s;
  ifstream inFile ( genome_to_load );
  int ind = 0;
  bool done_this_flag = false;
  double fitness = 0.0;
  
  while(getline(inFile, s)){
    istringstream ss(s);
    
    if( ind == which_genotype ){
      for(int ind = 0; ind < num_agents; ind++) {
	ss >> *genotype_length;
	if( !done_this_flag ){
	  genes.assign((*genotype_length*num_agents), 0 );
	  done_this_flag = true;
	}
	for(int g=0; g<(*genotype_length); g++){
	  ss >> genes[g+((*genotype_length)*ind)];
      	}
      }
      ss >> fitness;
      break;
    }
    else{
      ind++;
    }
  }

  if( genes.size() == 0 ){
    cerr << " File name = " << str_source << " ID = " << which_genotype << " NOT FOUND " << endl;
    exit(0);
  }
}

/* ------------------------------------------------------------------------------------------ */


class Controller {
  
 protected:
  double delta_t;
  static const int   num_input = 8;
  static const int   num_output = 4;
  int   genotype_length;
  
 public:
  Controller() {};
  virtual ~Controller(){};
  
  int         get_num_input           ( void )          { return num_input; }
  int         get_num_output          ( void )          { return num_output; }
  inline int  get_genotype_length     ( void )          { return genotype_length; }
  inline void set_genotype_length     ( const int val ) { genotype_length = val; }
  
  virtual void init                     ( const vector <chromosome_type> &genes ) = 0;
  virtual void step                     ( const vector <double> &input_array, vector <double> &output_array) = 0;
  virtual void reset                    ( void ) = 0;
  virtual void read_from_file           ( void ) = 0; 
  virtual void compute_genotype_length  ( void ) = 0;
  
#ifdef _DIPLOID_
  inline double get_value ( const vector <double> &genes, const int counter ){
    return ((genes[counter]+genes[genotype_length+counter])/2.0);
  }
  inline double get_value ( const vector <int> &genes, const int counter ){
    return ((double)(genes[counter]+genes[genotype_length+counter])/DOUBLE_MAX_INT);
  }
#else
  inline double get_value ( const vector <double> &genes, const int counter){
    return genes[counter];
  }
  inline double get_value ( const vector <int> &genes, const int counter){
    return ((double)(genes[counter])/MAX_INT);
  }
#endif
};

#endif

