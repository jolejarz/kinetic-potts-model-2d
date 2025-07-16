/**************************************************************
***************************************************************
*** 
***   run.h
***
***   Jason Olejarz
***   2011-01-26
***
***   This is the header file for run.c.
***
***************************************************************
**************************************************************/

struct run_parameters
{
	struct rng_struct *rng_struct;
	short int q;
	int xlen;
	int ylen;
	unsigned long int maxsteps;
	double maxtime;
	short int *spins_initial;
	short int *spins;
	short int *state_initial;
	short int *state;
	int *map[11+MAX_Q*(MAX_Q-1)];
	int *index[11+MAX_Q*(MAX_Q-1)];
	double prob[11+MAX_Q*(MAX_Q-1)];
	int coordinate;
	short int move;
	short int move_backward;
	unsigned long int steps;
	double time;
	double energy;			// This is the total energy of the system.
	int mag;			// This is the total magnetization of the system.
	int num_value[MAX_Q];		// This is the total number of spins of each value.
	short int *T_vertices;		// This is not essential for doing the simulation.
	short int *T_vertices_2;	// This is not essential for doing the simulation.
	int num_energy_drops;		// This is the total number of energy drops each time `run' is executed.
};

short int run (short int (*step[])(), short int step_size, void *data[], void *data_parameters[], short int data_size, struct run_parameters *parameters, short int extra_time_for_blinkers);
