/**************************************************************
***************************************************************
*** 
***   cmd-set.c
***
***   Jason Olejarz
***   2011-01-26
***
***   This file contains commands for setting parameters.
***
***************************************************************
**************************************************************/

void cmd_block()
{
	struct run_parameters *parameters;
	int *parameters_size;

	if (data_input(data, data_parameters, &parameters, &parameters_size, arg_short_int[0], data_num)==1) return result_error;

	set_block(parameters, arg_short_int[1], arg_int[0], arg_int[1], arg_int[2], arg_int[3]);

	return;
}

void cmd_cpcur()
{
	short int data_index=arg_short_int[0];
	
	struct run_parameters *p;
	int *p_size;
	
	if (data_input(data, data_parameters, &p, &p_size, data_index, data_num)==1) return result_error;
	
	memcpy (p->spins_initial, p->spins, sizeof(short int)*(p->xlen)*(p->ylen));

	return;
}

void cmd_cpinit()
{
	short int data_index=arg_short_int[0];
	
	struct run_parameters *p;
	int *p_size;
	
	if (data_input(data, data_parameters, &p, &p_size, data_index, data_num)==1) return result_error;
	
	memcpy (p->spins, p->spins_initial, sizeof(short int)*(p->xlen)*(p->ylen));
	
	return;
}

void cmd_rnglcg()
{
	short int data_index=arg_short_int[0];
	
	struct rng_struct *rng_struct;
	int *rng_struct_size;
	
	// Allocate a block of memory to store info for the random number generator.
	if (data_output(data, data_parameters, &rng_struct, &rng_struct_size, sizeof(struct rng_struct), sizeof(int), data_index, data_num)==1) return result_error;
	
	rng_struct->rng_alloc=rng_alloc_lcg;
	rng_struct->rng_seed=rng_seed_lcg;
	rng_struct->rng_next=rng_next_lcg;
	rng_struct->rng_backup=rng_backup_lcg;
	rng_struct->rng_restore=rng_restore_lcg;

	rng_struct->rng_alloc(rng_struct);
	
	return;
}

void cmd_rngmt()
{
	short int data_index=arg_short_int[0];
	
	struct rng_struct *rng_struct;
	int *rng_struct_size;
	
	// Allocate a block of memory to store info for the random number generator.
	if (data_output(data, data_parameters, &rng_struct, &rng_struct_size, sizeof(struct rng_struct), sizeof(int), data_index, data_num)==1) return result_error;
	
	rng_struct->rng_alloc=rng_alloc_mt;
	rng_struct->rng_seed=rng_seed_mt;
	rng_struct->rng_next=rng_next_mt;
	rng_struct->rng_backup=rng_backup_mt;
	rng_struct->rng_restore=rng_restore_mt;

	rng_struct->rng_alloc(rng_struct);
	
	return;
}

void cmd_seed()
{
	set_seed(data[arg_short_int[0]], arg_double[0], arg_double[1]);

	return;
}

void cmd_set()
{
	short int data_index=arg_short_int[0];
	short int data_index_rng=arg_short_int[1];
	int xlen=arg_int[0];
	int ylen=arg_int[1];
	unsigned long int maxsteps=arg_unsigned_long_int[0];
	double maxtime=arg_double[0];
	short int q=arg_short_int[2];
	
	static struct run_parameters *parameters;
	static int *parameters_size;
	
	int i, x, y;

	// Allocate a block of memory to store parameter values.
	if (data_output(data, data_parameters, &parameters, &parameters_size, sizeof(struct run_parameters), sizeof(int), data_index, data_num)==1) return result_error;

	// Fill in the parameters.
	parameters->rng_struct=data[data_index_rng];
	parameters->xlen=xlen;
	parameters->ylen=ylen;
	parameters->maxsteps=maxsteps;
	parameters->maxtime=maxtime;
	parameters->q=q;

	// Allocate memory.
	parameters->spins_initial=(short int*)malloc (sizeof(short int)*xlen*ylen);
	parameters->spins=(short int*)malloc (sizeof(short int)*xlen*ylen);
	parameters->state_initial=(short int*)malloc (sizeof(short int)*xlen*ylen);
	parameters->state=(short int*)malloc (sizeof(short int)*xlen*ylen);
	for (i=0; i<11+q*(q-1); i++)
	{
		parameters->map[i]=(int*)malloc (sizeof(int)*xlen*ylen);
		parameters->index[i]=(int*)malloc (sizeof(int)*(1+xlen*ylen));
	}

	parameters->T_vertices=(short int*)malloc (sizeof(short int)*xlen*ylen);
	parameters->T_vertices_2=(short int*)malloc (sizeof(short int)*xlen*ylen);

	return;
}

void cmd_settsc()
{
	set_settsc(data[arg_short_int[0]]);
	return;
}

