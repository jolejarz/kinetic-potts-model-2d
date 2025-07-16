/**************************************************************
***************************************************************
*** 
***   loop.h
***
***   Jason Olejarz
***   2011-01-26
***
***   This is the header file for loop.c.
***
***************************************************************
**************************************************************/

struct loop_iterate_data
{
	int count;
	short int progress;
};

struct loop_randdiff_data
{
	short int data_index_parameters;
};

struct loop_randq_data
{
	short int data_index_parameters;
	int rho_numerator[MAX_Q-1];
	int rho_denominator[MAX_Q-1];
};

struct loop_seed_data
{
	struct rng_struct *rng_struct;
	short int data_index_low;
	short int data_index_high;
	int num;
};

struct loop_seed_tsc_data
{
	struct rng_struct *rng_struct;
};

short int loop_iterate(short int command, struct loop_iterate_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int loop_rand_diff(short int command, struct loop_randdiff_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int loop_randq(short int command, struct loop_randq_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int loop_randq_equal(short int command, struct loop_randq_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int loop_seed(short int command, struct loop_seed_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int loop_seed_tsc(short int command, struct loop_seed_tsc_data *command_data, void *data[], void *data_parameters[], short int data_size);

