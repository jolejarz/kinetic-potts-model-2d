/**************************************************************
***************************************************************
*** 
***   set.h
***
***   Jason Olejarz
***   2011-01-26
***
***   This is the header file for set.c.
***
***************************************************************
**************************************************************/

enum {set_error, set_ok};

short int set_block(struct run_parameters *parameters, short int spin, int xi, int xf, int yi, int yf);
short int set_rand_diff(struct run_parameters *parameters);
short int set_rand_q(struct run_parameters *parameters, int *index, int rho_numerator[], int rho_denominator[]);
short int set_rand_q_equal(struct run_parameters *parameters, int *index);
short int set_seed(struct rng_struct *rng_struct, double seed_low, double seed_high);
short int set_settsc(struct rng_struct *rng_struct);

