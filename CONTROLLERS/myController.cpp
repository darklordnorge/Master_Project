#include "myController.h"

/*Contructor*/
MyController::MyController(){
    compute_genotype_length();
}

void MyController::compute_genotype_length ( void ){
    /*Input and hiddenlayer each have a BIAS node therefor + 1 */
    genotype_length = (((num_input+1) * hiddenlayer_size) + ((hiddenlayer_size + 1) * num_output));

}

/* -------------------------------------------------------------------------------------- */

void MyController::init ( const vector <chromosome_type> &genes ){
    vector <double> new_gene;//vector to hold the scaled gene values
    /*initalize the layers*/
    inputlayer.assign(num_input+1, 0.0);
    hiddenlayer.assign(hiddenlayer_size+1, 0.0);
    outputlayer.assign(num_output, 0.0);

    new_gene.resize(genes.size());

    /*initialise the input-to-hiddenlayer weights*/
    for(int i = 0;i < num_input+1;i++){
        for(int j = 0;j < hiddenlayer_size;j++){
            weights1[i][j] = 0.0;
        }
    }
    /*initialise the hidden-to-outputlayer weights*/
    for(int m = 0;m < hiddenlayer_size+1;m++){
        for(int n = 0;n < num_output;n++){
            weights2[m][n] = 0.0;
        }
    }

    /*scale the genes from -5 to 5*/
    for(int i = 0;i < genes.size();i++){
        new_gene[i] = ((high_bound - low_bound) * get_value(genes, i)) + low_bound;
    }

    /*set the weights*/
    int counter = 0;
    for(int i = 0;i < num_input+1;i++){
        for(int j = 0;j < hiddenlayer_size;j++){
            weights1[i][j] = new_gene[counter];
            counter++;
        }
    }
    for(int m = 0;m < hiddenlayer_size+1;m++){
        for(int n = 0;n < num_output;n++){
            weights2[m][n] = new_gene[counter];
            counter++;
        }
    }
}

/* -------------------------------------------------------------------------------------- */

void MyController::step ( const vector <double> &input, vector <double> &output){

    //update the network

}

/* -------------------------------------------------------------------------------------- */

void MyController::reset ( void ){

    // reset the state of neurons

}


/* -------------------------------------------------------------------------------------- */
