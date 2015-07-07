#include "roulette_wheel.h"

/* -------------------------------------------------------------------------------------- */

Roulette_wheel::Roulette_wheel ( const int num_alleles_, const int num_bases_per_allele_, const int num_agents_per_group_ ) : GA( num_agents_per_group_) {

  chromosome        = new Chromosome <chromosome_type> *[num_groups/* population size*/];
  tmp_chromosome    = new Chromosome <chromosome_type> *[num_groups];
  for (int g = 0; g < num_groups; g++){
    chromosome[g]     = new Chromosome <chromosome_type> [num_agents_per_group];
    tmp_chromosome[g] = new Chromosome <chromosome_type> [num_agents_per_group];
    for(int ind = 0; ind < num_agents_per_group; ind++){
      chromosome[g][ind].set_diploid ( DIPLOID_FLAG );
      chromosome[g][ind].init_allele_values ( num_alleles_, num_bases_per_allele_ );
      tmp_chromosome[g][ind].set_diploid ( DIPLOID_FLAG );
    }
  }
  
  wheel.assign (num_tot_solutions, 0);
  fitness.clear();
  
#ifdef _RANK_BASED_
  linear_rank_wheel();
#endif
}

/* -------------------------------------------------------------------------------------- */

Roulette_wheel::~Roulette_wheel ( void ){
  for (int g = 0; g < num_groups; g++){
    delete[] chromosome[g];
    delete[] tmp_chromosome[g];
  }
  delete[] chromosome;
  delete[] tmp_chromosome;
}

/* -------------------------------------------------------------------------------------- */

void Roulette_wheel::assign_fitness( const int ind, const vector <double> & final_fitness ){
  if( ind == 0 ){
    fitness.clear();
    wheel[ind] = 0.0;
    fitness.push_back( MakeValueWithIndex(final_fitness[0], ind) );
  }
  else {
    wheel[ind] = 0.0;
    fitness.push_back( MakeValueWithIndex(final_fitness[0], ind) );
    if( ind == (num_tot_solutions-1) ){ 
      sort(fitness.begin(), fitness.end()); 
#ifndef _RANK_BASED_
      compute_cumulative_fitness ( );
#endif
    }
  }
}

/* -------------------------------------------------------------------------------------- */

void Roulette_wheel::breeding   ( void ){
  
#ifdef _HETERO_INDIVIDUAL_SELECTION_
  //breeding_hetero_individual_selection();
#else
  breeding_group_selection();
#endif
}

/* -------------------------------------------------------------------------------------- */

void Roulette_wheel::breeding_group_selection ( void ){
  for(int g = 0; g < num_groups; g++){
    if( g < num_elite ){
      for(int ind = 0; ind < num_agents_per_group; ind++){
	tmp_chromosome[g][ind] = chromosome[fitness[num_tot_solutions-1-g].index][ind]; //since sorting is descending order for fitness
      }
    }
    else{
      for(int ind = 0; ind < num_agents_per_group; ind++){
	int mum, first_ind; 
	select_group ( &mum ); //select index of indivdual mum from fitness vector using roulette wheel propability 
#ifdef _HETERO_GROUP_SELECTION_
	first_ind = gsl_rng_uniform_int (GSL_randon_generator::r_rand, num_agents_per_group );
#else
	first_ind = 0; 
#endif
	if( gsl_rng_uniform(GSL_randon_generator::r_rand)  < prob_cross_over ) {
	  int dad, second_ind;
	  select_second_group( &mum, &dad );
#ifdef _HETERO_GROUP_SELECTION_
	  second_ind = gsl_rng_uniform_int (GSL_randon_generator::r_rand, num_agents_per_group );
#else
	  second_ind = 0;
#endif
	  tmp_chromosome[g][ind].create_with_cross_over_and_mutate_operators ( chromosome[mum][first_ind], 
									       chromosome[dad][second_ind], &prob_mutation );
	}
	else{
	  tmp_chromosome[g][ind].create_with_mutate_operator ( chromosome[mum][first_ind], &prob_mutation );
	}
      }
    }
  }
  
  for(int g = 0; g < num_groups; g++){
    for(int ind = 0; ind < num_agents_per_group; ind++){
      chromosome[g][ind] = tmp_chromosome[g][ind];
    }
  }
}

/* -------------------------------------------------------------------------------------- */

void Roulette_wheel::select_group( int *mum ){
  // no idea why assigning mum to value which didnt used
  *mum = gsl_rng_uniform_int (GSL_randon_generator::r_rand, num_tot_solutions );
  //select random propability number as reference for seleting individual from population
  double reference = gsl_rng_uniform(GSL_randon_generator::r_rand) * wheel[num_tot_solutions-1];   
  for (int selected = num_solutions_truncated; selected  < num_tot_solutions; selected++ ){
    if(reference < wheel[selected] ){
      *mum = fitness[selected].index;
      break;
    }
  }
}

/* --------------------------------------------------------------------------------------- */

void Roulette_wheel::select_second_group(const int *mum, int *dad ){
  *dad = *mum;
  int iter = 0;
  do{
    select_group( dad );
    iter++;
  }while(*mum == *dad && iter < 5);
}

/* -------------------------------------------------------------------------------------- */

// This function calculate the roulette wheel slot size from the fitness of individuals in 
// the population excluding the truncated number

void Roulette_wheel::compute_cumulative_fitness ( void ){ 
  double tmp_fit = 0.0;
  for(int i = 0; i < num_tot_solutions; i++){      
    if( i < num_solutions_truncated ){
      wheel[i] = 0.0;
      tmp_fit  = fitness[i].value;
    }
    else if ( i == num_solutions_truncated ){
      wheel[i] = fitness[i].value - tmp_fit;
    }
    else{
      wheel[i] += wheel[i-1] + ( fitness[i].value - tmp_fit);
    }
    //cerr << " Chrom = " << i << " fitness["<<i<<"].value = " << fitness[i].value << " fitness["<<i<<"].index = " << fitness[i].index
    //	 << " Wheel["<<i<<"] = " << wheel[i] << endl;
    //getchar();
  }
}

/* -------------------------------------------------------------------------------------- */

#ifdef _RANK_BASED_
void Roulette_wheel::linear_rank_wheel( void ){
  double selective_pressure = 2.0;
  for (int position = 0; position < num_tot_solutions; position++){
    if (position <= (num_solutions_truncated-1) ) {
      wheel[position] = 0.0;
    }
    else {
      wheel[position] = ((2.0 - selective_pressure) + 
			 2.0*(selective_pressure - 1.0)*((double)(position - num_solutions_truncated )/(double)(num_tot_solutions-1-num_solutions_truncated)) );
    }
  }
}
#endif

/* -------------------------------------------------------------------------------------- */

void Roulette_wheel::dump_genome_into_file( const char *dir, const char *fileName, const int generation ) {
  char fname_genome[500];
  sprintf(fname_genome, "%s%s_pop_G%d.geno", dir, fileName, generation );
  ofstream out ( fname_genome );
  out.setf( ios::fixed );
  
#ifndef _HETERO_INDIVIDUAL_SELECTION_
  for(unsigned int g=0; g<num_groups; g++) {
    for(unsigned int ind = 0; ind < num_agents_per_group; ind++) {
      out << " " << chromosome[fitness[num_tot_solutions-1-g].index][ind].get_allele_values().size();
      for(int n=0; n<chromosome[fitness[num_tot_solutions-1-g].index][ind].get_allele_values().size(); n++){
	out << " " << setprecision(15) << chromosome[fitness[num_tot_solutions-1-g].index][ind].get_allele_values()[n]; 
      }
    }
    out << " " << fitness[num_tot_solutions-1-g].value;
    out	<< endl;
  }
  out.close();
#endif
}

/* -------------------------------------------------------------------------------------- */

void Roulette_wheel::upload_genome_from_file( const char *dir, const char *fileName, const int generation ) {
  char fname_genome[500];
  sprintf(fname_genome, "%s%s_pop_G%d.geno", dir, fileName, generation );
 
  ifstream inFile (fname_genome );
  inFile.setf(ios::fixed );
  string s;
  int num_g = 0;
  int genotype_lenght;
  vector <chromosome_type> genes;
  genes.assign(chromosome[0][0].get_allele_values().size(), 0);
  
  while(getline(inFile, s) && ( num_g < num_groups ) ){
    istringstream ss(s);
    
    for(unsigned int ind = 0; ind < num_agents_per_group; ind++) {
      ss >> genotype_lenght;
      for(int q=0; q<genes.size(); q++){
	ss >> genes[q];
      }
      chromosome[num_g][ind].set_allele_values( genes );
    }
    
    num_g++;
  }
  fitness.clear();
}

/* -------------------------------------------------------------------------------------- */
//                                       END
/* -------------------------------------------------------------------------------------- */

