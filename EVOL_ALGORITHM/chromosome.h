#ifndef GENE_H
#define GENE_H

#include <stdlib.h>
#include <math.h>
#include <vector>
#include "../MISC/general.h"

#define STD  0.1

using namespace std;

/* ------------------------------------------------------------------------------------------ */
// Gene Base class
/* ------------------------------------------------------------------------------------------ */
template <class T>
class BasesT {
  
 public:

  /* Init a random base in between (0, 1] */
  T  get_new_value  ( void ) {
    return (T)(gsl_rng_uniform_pos( GSL_randon_generator::r_rand ));
  }

  T get_new_binary_value( void ){
    return (T)(gsl_rng_uniform_int(GSL_randon_generator::r_rand, 2) );
  }

  void mutate_base_binary_value ( T *base_, double *prob_mutation ){
    if( gsl_rng_uniform( GSL_randon_generator::r_rand ) < *prob_mutation ) {
      if( *base_ == 1.0 ) *base_ = 0.0;
      else *base_ = 1.0;
    }
  }
  
  /* Mutate the base within [0, 1] */
  void mutate_base ( T *base_, double *prob_mutation ){
    if( gsl_rng_uniform( GSL_randon_generator::r_rand ) < *prob_mutation ) {
      T offset    = (T)(gsl_ran_gaussian( GSL_randon_generator::r_rand, STD));
      T new_value = ( *base_ + offset );
      if( new_value > 1 ) {
	*base_ = 1;
      }
      else {
	if ( new_value < 0 ) {
	  *base_ = 0;
	}
	else {
	  *base_ = new_value;
	}
      }
    }
  }
};
/* ------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------------------------ */
//Gene Base Class Specialisation (int)
/* ------------------------------------------------------------------------------------------ */
template <>
class BasesT <int> {
  
 public:
  
  /* Init a random binary base */
  int  get_new_value  ( void ){
    return (int)((gsl_rng_uniform_int(GSL_randon_generator::r_rand, 256) & 0xFF) );//generate a random int in 0 to (n-1) inclusive
  }
  
  int get_new_binary_value( void ){
    return gsl_rng_uniform_int(GSL_randon_generator::r_rand, 2);
  }

  void mutate_base_binary_value ( int *base_, double *prob_mutation ){
    if( gsl_rng_uniform( GSL_randon_generator::r_rand ) < *prob_mutation ) {
      if( *base_ == 1.0 ) *base_ = 0;
      else *base_ = 1.0;
    }
  }

  /* Mutate the base */
  void mutate_base ( int *base_, double *prob_mutation ){
    for (int x = 0; x < 8; x++){
      if( gsl_rng_uniform(GSL_randon_generator::r_rand) < *prob_mutation ) {
	int my_mask = 1; // 00 00 00 01
	my_mask = (my_mask << x); //swifth the 1 of x position on the left
	*base_ = (*base_ ^ my_mask); // this is the XOR operator
      }
    }
  }
  
};
/* ------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------ */
template <class T> 
class Chromosome {
  
 private:
  bool diploid;
  vector <T> allele;
  BasesT <T> base_value;
  
 public:
  //Class Constructor
  Chromosome( void ){  }
		     
  friend class Roulette_wheel;
  friend class Tournament;
  friend class Swarm_ga;

  //Copy constructor
  Chromosome(const Chromosome <T> &obj)
    {
      this->allele.resize( obj.allele.size(), 0 );
      for (unsigned int a = 0; a < this->allele.size(); a++){
	allele[a] = obj.allele[a];
      }
    }
  
  //Create a new assign operator
  Chromosome <T> & operator= (const Chromosome <T> &other)
    {
      this->allele.clear();
      if (this != &other) // protect against invalid self-assignment
        {
	  this->allele.assign ( other.allele.size(), 0 );
	  for (unsigned int a = 0; a < this->allele.size(); a++){
	    this->allele[a] = other.allele[a];
	  }
	}
      return *this;
    }
  
  void print_allele( void ){
    for (unsigned int a = 0; a < this->allele.size(); a++){
      cerr << "allele["<<a<<"] = " << (int) allele[a] << endl;
    }
  }
  
  inline void set_diploid ( bool d ) {diploid = d;}

  vector <T> & get_allele_values( void ){
    return allele;
  }

  void set_allele_values( vector <T> & alleles_){
    for (int a=0; a < alleles_.size(); a++)
      allele[a] = alleles_[a];
  }
  
  /* ---------------------------------------------------------------------------- */
  /*                        RESIZE ALLELE VECTOR FUNCTION                         */
  /* ---------------------------------------------------------------------------- */
  void resize_chromosome( const int num_alleles_, const int num_bases_per_allele_ ) {
    allele.clear();
    if( diploid ) allele.resize( 2.0 * num_alleles_ * num_bases_per_allele_ );
    else allele.resize( num_alleles_ * num_bases_per_allele_ );
  }
  
  /* ---------------------------------------------------------------------------- */
  /*                                INIT ALLELE FUNCTION                          */
  /* ---------------------------------------------------------------------------- */
  void init_allele_values( const int num_alleles_, const int num_bases_per_allele_ ) {
    resize_chromosome( num_alleles_, num_bases_per_allele_ );
    //cerr << " " << allele.size() << endl; 
    for (unsigned int a = 0; a < allele.size(); a++){
     // if( a >= (allele.size() - 20) )  {
	//allele[a] =  this->base_value.get_new_binary_value( );
	//cerr << " Binary allele["<<a<<"] = " << allele[a] << endl;
      //}
     // else {
	allele[a] =  this->base_value.get_new_value( );
	//cerr << " allele["<<a<<"] = " << allele[a] << endl;
      //}
    }
    //getchar();
  }
  
  /* ---------------------------------------------------------------------------- */
  /*                                MUTATION FUNCTION                             */
  /* ---------------------------------------------------------------------------- */
  void create_with_mutate_operator ( const Chromosome <T> &other, double *prob_mutation ){
    allele.clear();
    allele.resize( other.allele.size() );
    for (unsigned int a = 0; a < this->allele.size(); a++){
      allele[a] = other.allele[a];
#ifdef _HEBBIAN_
      //HEBBIAN STUFF
      if( a < (allele.size()-20) ) base_value.mutate_base ( &allele[a], prob_mutation );
      else base_value.mutate_base_binary_value ( &allele[a], prob_mutation );
#else
      base_value.mutate_base ( &allele[a], prob_mutation );
#endif
    }
  }
  
  /* ---------------------------------------------------------------------------- */
  /*                               CROSSOVER FUNCTIONS                            */
  /* ---------------------------------------------------------------------------- */
  
  void create_with_cross_over_and_mutate_operators ( const Chromosome <T> &mum, const Chromosome <T> &dad, double *prob_mutation ){
    this->allele.clear();
    if( diploid ) { //the child is DIPLOID (d)
      if( dad.diploid ){ //the mum is DIPLOID (d) - the dad is DIPLOID (d)
        ddd_crossover( mum, dad, prob_mutation );
      }
      else if( !dad.diploid ){ //the mum is DIPLOID (d) - the dad is APLOID (a)
	dda_crossover( mum, dad, prob_mutation );
      }
    }
    else{ //the child is APLOID (a)
      if( !mum.diploid && !dad.diploid ){
	aaa_crossover( mum, dad, prob_mutation );//the mum is APLOID (a) - the dad is APLOID (a)
      }
      else if ( mum.diploid ) {
	adx_crossover( mum, dad, prob_mutation );//the mum is DIPLOID (d) - the dad is IRRELEVANT (x)
      }
    }
  } 
  
  /* ---------------------------------------------------------------------------- */
  /* FIRST CASE :: The child is DIPLOID - the mum is DIPLOID - the dad is DIPLOID */
  /* ---------------------------------------------------------------------------- */
  void ddd_crossover ( const Chromosome <T> &mum, const Chromosome <T> &dad, double *prob_mutation ){
    allele.resize( mum.allele.size() );
    int half = (int)(mum.allele.size()*0.5);
#ifdef _HEBBIAN_
    //HEBBIAN STUFF
    double non_binary_alleles = (allele.size() - 20);
#endif
    
    for (unsigned int a = 0; a < this->allele.size(); a++){
      if ( a <  half ) {
	if( gsl_rng_uniform(GSL_randon_generator::r_rand) < 0.5 ) {
	  this->allele[a] = mum.allele[a];
#ifdef _HEBBIAN_
	  //HEBBIAN STUFF
	  if( a < non_binary_alleles ) base_value.mutate_base ( &allele[a], prob_mutation );
	  else base_value.mutate_base_binary_value ( &allele[a], prob_mutation );
#else
	  base_value.mutate_base ( &allele[a], prob_mutation );
#endif
	}
	else{
	  this->allele[a] = mum.allele[half + a];
#ifdef _HEBBIAN_
	  //HEBBIAN STUFF
	  if( a < non_binary_alleles ) base_value.mutate_base ( &allele[a], prob_mutation );
	  else base_value.mutate_base_binary_value ( &allele[a], prob_mutation );
#else
	  base_value.mutate_base ( &allele[a], prob_mutation );
#endif
	}
      }
      else {
	if( gsl_rng_uniform(GSL_randon_generator::r_rand) < 0.5 ) {
	  this->allele[a] = dad.allele[a - half];
#ifdef _HEBBIAN_
	  //HEBBIAN STUFF
	  if( a < non_binary_alleles ) base_value.mutate_base ( &allele[a], prob_mutation );
	  else base_value.mutate_base_binary_value ( &allele[a], prob_mutation );
#else
	  base_value.mutate_base ( &allele[a], prob_mutation );
#endif
	}
	else{
	  this->allele[a] = dad.allele[a];
#ifdef _HEBBIAN_
	  //HEBBIAN STUFF
	  if( a < non_binary_alleles ) base_value.mutate_base ( &allele[a], prob_mutation );
	  else base_value.mutate_base_binary_value ( &allele[a], prob_mutation );
#else
	  base_value.mutate_base ( &allele[a], prob_mutation );
#endif
	}
      }
    }
  }

  /* ---------------------------------------------------------------------------- */
  /* SECOND CASE :: The child is DIPLOID - the mum is DIPLOID - the dad is APLOID */
  /* ---------------------------------------------------------------------------- */
  void dda_crossover ( const Chromosome <T> &mum, const Chromosome <T> &dad, double *prob_mutation ){
    allele.resize( mum.allele.size() );
    int half = (int)(mum.allele.size()*0.5);
#ifdef _HEBBIAN_
    //HEBBIAN STUFF
    double non_binary_alleles = (allele.size() - 20);
#endif
    for (unsigned int a = 0; a < this->allele.size(); a++){
      if ( a <  half ) {
	if( gsl_rng_uniform(GSL_randon_generator::r_rand) < 0.5 ) {
	  this->allele[a] = mum.allele[a];
#ifdef _HEBBIAN_
	  //HEBBIAN STUFF
	  if( a < non_binary_alleles ) base_value.mutate_base ( &allele[a], prob_mutation );
	  else base_value.mutate_base_binary_value ( &allele[a], prob_mutation );
#else
	  base_value.mutate_base ( &allele[a], prob_mutation );
#endif
	}
	else{
	  this->allele[a] = mum.allele[half + a];
#ifdef _HEBBIAN_
	  //HEBBIAN STUFF
	  if( a < non_binary_alleles ) base_value.mutate_base ( &allele[a], prob_mutation );
	  else base_value.mutate_base_binary_value ( &allele[a], prob_mutation );
#else
	  base_value.mutate_base ( &allele[a], prob_mutation );
#endif
	}
      }
      else {
	this->allele[a] = dad.allele[a - half];
#ifdef _HEBBIAN_
	  //HEBBIAN STUFF
	  if( a < non_binary_alleles ) base_value.mutate_base ( &allele[a], prob_mutation );
	  else base_value.mutate_base_binary_value ( &allele[a], prob_mutation );
#else
	  base_value.mutate_base ( &allele[a], prob_mutation );
#endif
      }
      //cerr << " this->allele["<<a<<"] = " <<  (int)this->allele[a] << endl;
    }
  }

  /* ------------------------------------------------------------------------------ */
  /* THIRD CASE :: The child is APLOID - the mum is DIPLOID - the dad is IRRELEVANT */
  /* ------------------------------------------------------------------------------ */
  void adx_crossover ( const Chromosome <T> &mum, const Chromosome <T> &dad, double *prob_mutation ){
    int half = (int)(mum.allele.size()*0.5);
    allele.resize( half, 0 );
#ifdef _HEBBIAN_
    //HEBBIAN STUFF
    double non_binary_alleles =  (allele.size() - 20);
#endif
    for (unsigned int a = 0; a < this->allele.size(); a++){
      if( gsl_rng_uniform(GSL_randon_generator::r_rand) < 0.5 ) {
	this->allele[a] = mum.allele[a];
#ifdef _HEBBIAN_
	  //HEBBIAN STUFF
	  if( a < non_binary_alleles ) base_value.mutate_base ( &allele[a], prob_mutation );
	  else base_value.mutate_base_binary_value ( &allele[a], prob_mutation );
#else
	  base_value.mutate_base ( &allele[a], prob_mutation );
#endif
      }
      else{
	this->allele[a] = mum.allele[half + a];
#ifdef _HEBBIAN_
	//HEBBIAN STUFF
	if( a < non_binary_alleles ) base_value.mutate_base ( &allele[a], prob_mutation );
	else base_value.mutate_base_binary_value ( &allele[a], prob_mutation );
#else
	base_value.mutate_base ( &allele[a], prob_mutation );
#endif
      }
      //cerr << " this->allele["<<a<<"] = " <<  (int)this->allele[a] << endl;
    }
  }
  
  /* ------------------------------------------------------------------------ */
  /* FORTH CASE ::The child is APLOID - the mum is APLOID - the dad is APLOID */
  /* ------------------------------------------------------------------------ */
  void aaa_crossover ( const Chromosome <T> &mum, const Chromosome <T> &dad, double *prob_mutation ){
    allele.resize( dad.allele.size() );
    int cross_over_point = 1 + gsl_rng_uniform_int (GSL_randon_generator::r_rand, (allele.size()-1) );
#ifdef _HEBBIAN_
    //HEBBIAN STUFF
    int non_binary_alleles = (allele.size() - 20);
#endif
    
    if( gsl_rng_uniform(GSL_randon_generator::r_rand) < 0.5 ) {
      for (unsigned int a = 0; a < allele.size(); a++){
    	if ( a < cross_over_point ) {
    	  allele[a] = dad.allele[a];
#ifdef _HEBBIAN_
	  //HEBBIAN STUFF
	  if( a < non_binary_alleles ) base_value.mutate_base ( &allele[a], prob_mutation );
	  else base_value.mutate_base_binary_value ( &allele[a], prob_mutation );
#else
	  base_value.mutate_base ( &allele[a], prob_mutation );
#endif
    	}
    	else{
    	  allele[a] = mum.allele[a];
#ifdef _HEBBIAN_
	  //HEBBIAN STUFF
	  if( a < non_binary_alleles ) base_value.mutate_base ( &allele[a], prob_mutation );
	  else base_value.mutate_base_binary_value ( &allele[a], prob_mutation );
#else
	  base_value.mutate_base ( &allele[a], prob_mutation );
#endif
    	}
      }
    }
    else {
      for (unsigned int a = 0; a < this->allele.size(); a++){
	if ( a < cross_over_point ) {
	  allele[a] = mum.allele[a];
#ifdef _HEBBIAN_
	  //HEBBIAN STUFF
	  if( a < non_binary_alleles ) base_value.mutate_base ( &allele[a], prob_mutation );
	  else base_value.mutate_base_binary_value ( &allele[a], prob_mutation );
#else
	  base_value.mutate_base ( &allele[a], prob_mutation );
#endif
	}
	else{
	  allele[a] = dad.allele[a];
#ifdef _HEBBIAN_
	  //HEBBIAN STUFF
	  if( a < non_binary_alleles ) base_value.mutate_base ( &allele[a], prob_mutation );
	  else base_value.mutate_base_binary_value ( &allele[a], prob_mutation );
#else
	  base_value.mutate_base ( &allele[a], prob_mutation );
#endif
	}
      }
    }
  }
  
};

/* ------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------ */

#endif
