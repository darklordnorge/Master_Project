#ifndef _MYCONTROLLER_
#define _MYCONTROLLER_

#include "controller.h"

class MyController : public Controller {

 private:
    static const int hiddenlayer_size = 3;
    double weights1[num_input+1][hiddenlayer_size];
    double weights2[hiddenlayer_size +1][num_output];
    vector <double> inputlayer;
    vector <double> outputlayer;
    vector <double> hiddenlayer;
    const static double high_bound = 5.0;
    const static double low_bound  = -5.0;

  // here is you data members
 public:
    MyController();
  /* -------------------------------------------------------------------------------------------------- */
  /*                                             VIRTUAL FUNCTIONS                                      */
  /* -------------------------------------------------------------------------------------------------- */
  void init                          ( const vector <chromosome_type> &genes );
  void step                          ( const vector <double> &input_array, vector <double> &output_array);
  void reset                         ( void );
  void compute_genotype_length       ( void );
  void read_from_file                ( void );
  int get_num_input                   (void);
  int get_num_output                  (void);
  /* -------------------------------------------------------------------------------------------------- */
  /* -------------------------------------------------------------------------------------------------- */
  // and any other functions you might need

};

#endif
