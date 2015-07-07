#ifndef _MYCONTROLLER_
#define _MYCONTROLLER_

#include "controller.h"

class MyController : public Controller {

 private:

  // here is you data members
 public:
  /* -------------------------------------------------------------------------------------------------- */
  /*                                             VIRTUAL FUNCTIONS                                      */
  /* -------------------------------------------------------------------------------------------------- */
  void init                          ( const vector <chromosome_type> &genes );
  void step                          ( const vector <double> &input_array, vector <double> &output_array);
  void reset                         ( void );
  void compute_genotype_length       ( void );
  /* -------------------------------------------------------------------------------------------------- */
  /* -------------------------------------------------------------------------------------------------- */
  // and any other functions you might need

};

#endif
