/**************************************************************
***************************************************************
*** 
***   set.c
***
***   Jason Olejarz
***   2011-01-26
***
***   This file contains the routines used to set the parameters for 'evolve'.
***
***************************************************************
**************************************************************/

#include "includes.h"

short int set_block(struct run_parameters *parameters, short int spin, int xi, int xf, int yi, int yf)
{
	if (xi<1 || xf>parameters->xlen || yi<1 || yf>parameters->ylen)
	{
		printf("Invalid coordinates for the block.\n");
		return set_error;
	}
	
	int x, y, xlen, nxtimesny;
	
	xlen=parameters->xlen;
	nxtimesny=(parameters->xlen)*(parameters->ylen);
	
	for (y=yi; y<=yf; y++)
	{
		for (x=xi; x<=xf; x++)
		{
			*(parameters->spins_initial+(x-1)+(y-1)*xlen)=spin;
		}
	}
	
	return set_ok;
}

short int set_rand_diff(struct run_parameters *parameters)
{
	int area=(parameters->xlen)*(parameters->ylen);
	
	int i;

	for (i=0; i<area; i++)
	{
		*(parameters->spins_initial+i)=i;
	}
	
	return set_ok;
}

short int set_rand_q(struct run_parameters *parameters, int *index, int rho_numerator[], int rho_denominator[])
{
	long double largest_random_number_plus_1;
	long double random_number;
	
	// The largest random number that can be generated plus 1 is 2^64.
	largest_random_number_plus_1=(long double)pow(2,16);
	largest_random_number_plus_1*=(long double)pow(2,16);
	largest_random_number_plus_1*=(long double)pow(2,16);
	largest_random_number_plus_1*=(long double)pow(2,16);

	int xlen, ylen;
	xlen=parameters->xlen; ylen=parameters->ylen;

	int area=(parameters->xlen)*(parameters->ylen);
	
	int index_size;
	
	int i, j, k;
	short int initial_spin;
	int num_flips;
	double sum_of_fractions;

	int x, y;
	
	double cumulative_fractions[MAX_Q-1];

	initial_spin=0;
	
	for (i=0; i<area; i++)
	{
		*(parameters->spins_initial+i)=initial_spin;
	}
	
	for (i=0; i<area; i++)
	{
		*(index+i)=i;
	}

	index_size=area;

	sum_of_fractions=0;
	num_flips=0;
	for (i=0; i<(parameters->q)-1; i++)
	{
		sum_of_fractions+=rho_numerator[i]/(double)rho_denominator[i];
		num_flips+=(int)((area*rho_numerator[i])/(double)rho_denominator[i]);
	}

	for (i=0; i<(parameters->q)-1; i++)
	{
		cumulative_fractions[i]=0;
		if (i>0) cumulative_fractions[i]=cumulative_fractions[i-1]+(rho_numerator[i]/(double)rho_denominator[i])/sum_of_fractions;
		else cumulative_fractions[i]=(rho_numerator[i]/(double)rho_denominator[i])/sum_of_fractions;
	}

	for (i=0; i<num_flips; i++)
	{
		// Generate a random number.
		(parameters->rng_struct)->rng_next(parameters->rng_struct);
		random_number=((parameters->rng_struct)->current_double);

		j=(int)(index_size*random_number);
		
		for (k=0; k<(parameters->q)-1; k++)
		{
			if (i<(int)(num_flips*cumulative_fractions[k])) {*(parameters->spins_initial+(*(index+j)))=k+1; break;}
			else if (k==(parameters->q)-2) {*(parameters->spins_initial+(*(index+j)))=k+1; break;}
		}
		
		*(index+j)=*(index+index_size-1);

		index_size--;
	}
	
	return set_ok;
}

short int set_rand_q_equal(struct run_parameters *parameters, int *index)
{
	long double largest_random_number_plus_1;
	long double random_number;
	
	// The largest random number that can be generated plus 1 is 2^64.
	largest_random_number_plus_1=(long double)pow(2,16);
	largest_random_number_plus_1*=(long double)pow(2,16);
	largest_random_number_plus_1*=(long double)pow(2,16);
	largest_random_number_plus_1*=(long double)pow(2,16);

	int xlen, ylen;
	xlen=parameters->xlen; ylen=parameters->ylen;

	int area=(parameters->xlen)*(parameters->ylen);
	
	int index_size;
	
	int i, j, k;
	short int initial_spin;
	int num_flips;
	double sum_of_fractions;

	int x, y;
	
	double cumulative_fractions[MAX_Q-1];

	initial_spin=0;
	
	for (i=0; i<area; i++)
	{
		*(parameters->spins_initial+i)=initial_spin;
	}
	
	for (i=0; i<area; i++)
	{
		*(index+i)=i;
	}

	index_size=area;

	sum_of_fractions=0;
	num_flips=0;
	for (i=0; i<(parameters->q)-1; i++)
	{
		sum_of_fractions+=1/(double)(parameters->q);
		num_flips+=(int)(area/(double)(parameters->q));
	}

	for (i=0; i<(parameters->q)-1; i++)
	{
		cumulative_fractions[i]=0;
		if (i>0) cumulative_fractions[i]=cumulative_fractions[i-1]+(1/(double)(parameters->q))/sum_of_fractions;
		else cumulative_fractions[i]=(1/(double)(parameters->q))/sum_of_fractions;
	}

	for (i=0; i<num_flips; i++)
	{
		// Generate a random number.
		(parameters->rng_struct)->rng_next(parameters->rng_struct);
		random_number=((parameters->rng_struct)->current_double);

		j=(int)(index_size*random_number);
		
		for (k=0; k<(parameters->q)-1; k++)
		{
			if (i<(int)(num_flips*cumulative_fractions[k])) {*(parameters->spins_initial+(*(index+j)))=k+1; break;}
			else if (k==(parameters->q)-2) {*(parameters->spins_initial+(*(index+j)))=k+1; break;}
		}
		
		*(index+j)=*(index+index_size-1);

		index_size--;
	}
	
	return set_ok;
}

short int set_seed(struct rng_struct *rng_struct, double seed_low, double seed_high)
{
	unsigned long int seed;
	
	seed=(unsigned long int)seed_high;
	seed*=(unsigned long int)pow(2,16);
	seed*=(unsigned long int)pow(2,16);
	seed+=(unsigned long int)seed_low;
	rng_struct->rng_seed(rng_struct, seed);

	return set_ok;
}

short int set_settsc(struct rng_struct *rng_struct)
{
	unsigned int low, high;
	unsigned long int seed;
	
	asm ("rdtsc" : "=a" (low), "=d" (high));
	seed=(unsigned long int)high;
	seed*=(unsigned long int)pow(2,16);
	seed*=(unsigned long int)pow(2,16);
	seed+=(unsigned long int)low;
	rng_struct->rng_seed(rng_struct, seed);
	
	return set_ok;
}

