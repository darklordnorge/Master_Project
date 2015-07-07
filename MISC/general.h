#ifndef _GENERAL_
#define _GENERAL_

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <fstream>
#include <iostream>
#include <string.h>
#include <sstream>
#include <vector>
#include <algorithm>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

enum {WALL = 0, CYLINDER=1, SPHERE=2, LIGHT=3, BRICK = 4, ROBOT=5 };

/* ------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------ */
//#define _HEBBIAN_
//#define _DIPLOID_
//#define _HETERO_GROUP_SELECTION_
//#define _HETERO_INDIVIDUAL_SELECTION_

#define MAX_INT         255
#define DOUBLE_MAX_INT  (MAX_INT*2)

//typedef int chromosome_type;
typedef double chromosome_type;
/* ------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------ */

using namespace std;

double        getDouble(char sep, istream& I);
int           getInt(char sep, istream& I);
unsigned long getLongInt(char sep, istream& I);
void          getStr(char sep, istream& I, char* s);
char          getChar(char sep, istream& I);
bool          getYesNo(char sep, istream& I);
bool          isIntString(char* s);
int           string2Int(char* s);
bool          fileExists(char* F);
double        f_sigmoid( double x );

namespace GSL_randon_generator
{
  extern gsl_rng *r_rand;
  void init_generator ( const int root_seed );
  void free_generator ( void );
  void reset_seed     ( const int root_seed );
}


/* /\* ------------------------------------------------------------------------------------------*\/ */

/* template <class myType> */
/* void swap ( myType *a, myType *b) { */
/*   myType temp = *a; */
/*   *a = *b; */
/*   *b = temp; */
/* } */

/* /\* ------------------------------------------------------------------------------------------*\/ */

/* template <class myType> */
/* void shuffle_array ( myType arr[], int arr_size) { */
/*   for (int i = (arr_size-1); i> 0; i--){ */
/*     int j = (int)(gsl_rng_uniform_int(GSL_randon_generator::r_rand, i+1 )); */
/*     swap <myType> (&arr[i], &arr[j]); */
/*   } */
/* } */

/* /\* ------------------------------------------------------------------------------------------*\/ */

#endif
