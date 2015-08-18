#include "myController.h"

double MyController::inputlayer[];
double MyController::hiddenlayer[];
double MyController::outputlayer[];

/*Contructor*/
MyController::MyController() {
    compute_genotype_length();
}


void MyController::compute_genotype_length ( void ){
    /*Input and hiddenlayer each have a BIAS node therefor + 1 */
    genotype_length = (((num_input+1) * hiddenlayer_size) + ((hiddenlayer_size + 1) * num_output));


}

/* -------------------------------------------------------------------------------------- */

void MyController::init ( const vector <chromosome_type> &genes ){
//    vector <double> new_gene;//vector to hold the scaled gene values
    double new_gene[genes.size()];
    /*initalize the layers*/
    for(int i = 0;i < num_input+1;i++){
        inputlayer[i] = 0.0;
    }

    for(int i = 0;i < num_output;i++){
        hiddenlayer[i] = 0.0;
    }

    for(int i = 0;i < hiddenlayer_size+1;i++){
        outputlayer[i] = 0.0;
    }


//    new_gene.resize(genes.size());
    /*Set new_genes to 0.0*/
    for(int i = 0;i < genes.size();i++){
        new_gene[i] = 0.0;
    }

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
    /*set values for the inputlayer*/
    for(int i = 0;i < input.size();i++){
        inputlayer[i] = get_value(input, i);
    }

    /*set Bias for the hiddenlayer*/
    inputlayer[num_input] = 1.0;

    /*reset the outputlayer*/
    for(int i = 0;i < num_output;i++){
        outputlayer[i] = 0.0;
    }

    /*add the weights from input to hiddenlayer*/
    for(int i = 0;i < hiddenlayer_size;i++){
        for(int j = 0;j < num_input+1;j++){
            hiddenlayer[i] +=(inputlayer[j] * (weights1[j][i]));
        }
    }

    /*calculate the sigmoid for the hiddenlayer*/
    for(int i = 0;i < hiddenlayer_size;i++){
        hiddenlayer[i] = f_sigmoid(hiddenlayer[i]);
    }

    /*add the bias for the outputlayer*/
    hiddenlayer[hiddenlayer_size] = 1.0;

    /*add the weights from the hidden-to-outputlayer*/
    for(int i = 0;i < num_output;i++){
        for(int j = 0;j < hiddenlayer_size+1;j++){
            outputlayer[i] +=(hiddenlayer[j] * (weights2[j][i]));
        }
    }

    /*calculate the sigmoid for the outputlayer*/
    for(int i = 0;i < num_output;i++){
        output[i] = f_sigmoid(outputlayer[i]);
    }
}

/* -------------------------------------------------------------------------------------- */

void MyController::reset ( void ){

    // reset the state of neurons

}
/* -------------------------------------------------------------------------------------- */
void MyController::read_from_file(void) {
}
int MyController::get_num_input ( void ){
    return num_input;
}
int MyController::get_num_output ( void ){
    return num_output;
}