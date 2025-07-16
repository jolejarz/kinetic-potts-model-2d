/**************************************************************
***************************************************************
*** 
***   cmd-plot.c
***
***   Jason Olejarz
***   2011-01-27
***
***   This file contains graphics commands.
***
***************************************************************
**************************************************************/

void cmd_anrt()
{
	short int data_index_parameters=arg_short_int[0];
	short int data_index_flips=arg_short_int[1];
	short int data_index_energy=arg_short_int[2];
	short int data_index_time=arg_short_int[3];
	double time_begin=arg_double[0];
	double time_end=arg_double[1];
	double time_between_frames=arg_double[2];
	double delay=arg_double[3];
	double time_offset=arg_double[4];
	short int save_frames=arg_short_int[4];

	static struct run_parameters *parameters;
	static int *parameters_size;

	if (data_input(data, data_parameters, &parameters, &parameters_size, data_index_parameters, data_num)==1) return;

	short int *spins=malloc(sizeof(short int)*(parameters->xlen)*(parameters->ylen));
	
	memcpy(spins, parameters->spins_initial, sizeof(short int)*(parameters->xlen)*(parameters->ylen));
	
	int *flips, *flips_size;

	if (data_input(data, data_parameters, &flips, &flips_size, data_index_flips, data_num)==1) return;

	double *energy;
	int *energy_size;
	
	if (data_input(data, data_parameters, &energy, &energy_size, data_index_energy, data_num)==1) return;

	double *time;
	int *time_size;
	
	if (data_input(data, data_parameters, &time, &time_size, data_index_time, data_num)==1) return;

	// Determine the step that corresponds with 'time_begin'.
	int steps_begin=0;
	while (1)
	{
		if (*(time+steps_begin)>=time_begin) break;
		steps_begin++;
	}

	// Determine the step that corresponds with 'time_end'.
	// If 'time_end' is longer than the simulation lasted, then
	// 'steps_end' is set to the maximum number of steps.
	int steps_end=steps_begin;
	while (1)
	{
		if (*(time+steps_end)>=time_end || steps_end==*time_size/sizeof(double)-1) break;
		steps_end++;
	}

	state(spins, flips, steps_begin);

	// Save a plot of the spins at 'steps_begin'.
	arg_short_int[0]=data_index_parameters;
	arg_short_int[1]=data_index_flips;
	arg_int[0]=steps_begin;
	cmd_spins();
	
	FILE *data, *plot;

	data=fopen(arg_string[2], "w");
	plot=fopen(arg_string[3], "w");

	animate_real_time (spins, flips+2*steps_begin, energy+steps_begin, time+steps_begin, steps_begin, steps_end-steps_begin, time_between_frames, parameters->xlen, parameters->ylen, delay, time_offset, save_frames, arg_string[2], data, plot);
	
	fclose(plot);
	fclose(data);
	
	free(spins);

	return;
}

void cmd_anrt2()
{
	short int data_index_parameters=arg_short_int[0];
	short int data_index_flips=arg_short_int[1];
	short int data_index_energy=arg_short_int[2];
	short int data_index_time=arg_short_int[3];
	short int data_index_spins=arg_short_int[4];
	double starting_time=arg_double[0];
	double time_between_frames=arg_double[1];
	double delay=arg_double[2];
	short int save_frames=arg_short_int[5];

	static struct run_parameters *parameters;
	static int *parameters_size;

	if (data_input(data, data_parameters, &parameters, &parameters_size, data_index_parameters, data_num)==1) return;

	int *flips, *flips_size;

	if (data_input(data, data_parameters, &flips, &flips_size, data_index_flips, data_num)==1) return;

	double *energy;
	int *energy_size;
	
	if (data_input(data, data_parameters, &energy, &energy_size, data_index_energy, data_num)==1) return;

	double *time;
	int *time_size;
	
	if (data_input(data, data_parameters, &time, &time_size, data_index_time, data_num)==1) return;

	short int *spins;
	int *spins_size;
	
	if (data_input(data, data_parameters, &spins, &spins_size, data_index_spins, data_num)==1) return;

	FILE *data, *plot;

	data=fopen(arg_string[0], "w");
	plot=fopen(arg_string[1], "w");

	animate_real_time_2 (spins, flips, energy, time, (*flips_size)/sizeof(int)/2, starting_time, time_between_frames, parameters->xlen, parameters->ylen, delay, save_frames, arg_string[0], data, plot);
	
	fclose(plot);
	fclose(data);

	return;
}

void cmd_anstep()
{
	short int data_index_parameters=arg_short_int[0];
	short int data_index_flips=arg_short_int[1];
	short int data_index_flips_backward=arg_short_int[2];
	short int data_index_spins=arg_short_int[3];
	int xmin=arg_int[0];
	int xmax=arg_int[1];
	int ymin=arg_int[2];
	int ymax=arg_int[3];

	static struct run_parameters *parameters;
	static int *parameters_size;

	if (data_input(data, data_parameters, &parameters, &parameters_size, data_index_parameters, data_num)==1) return;

	int *flips, *flips_size;

	if (data_input(data, data_parameters, &flips, &flips_size, data_index_flips, data_num)==1) return;

	int *flips_backward, *flips_backward_size;

	if (data_input(data, data_parameters, &flips_backward, &flips_backward_size, data_index_flips_backward, data_num)==1) return;

	short int *spins;
	int *spins_size;
	
	if (data_input(data, data_parameters, &spins, &spins_size, data_index_spins, data_num)==1) return;

	FILE *data, *plot;

	data=fopen(arg_string[0], "w");
	plot=fopen(arg_string[1], "w");

	animate_step (spins, flips, flips_backward, (*flips_size)/sizeof(int)/2, parameters->xlen, parameters->ylen, xmin, xmax, ymin, ymax, arg_string[0], data, plot);
	
	fclose(plot);
	fclose(data);

	return;
}

void cmd_brplot()
{
	short int data_index_parameters=arg_short_int[0];
	
	static struct run_parameters *parameters;
	static int *parameters_size;
	
	if (data_input(data, data_parameters, &parameters, &parameters_size, data_index_parameters, data_num)==1) return result_error;

	FILE *data_1, *plot_1, *data_2, *plot_2, *data_3, *plot_3;
	
	data_1=fopen(arg_string[0], "w");
	plot_1=fopen(arg_string[1], "w");
	data_2=fopen(arg_string[2], "w");
	plot_2=fopen(arg_string[3], "w");
	data_3=fopen(arg_string[4], "w");
	plot_3=fopen(arg_string[5], "w");

	blinker_region_plot_matlab(parameters, arg_string[0], data_1, plot_1, arg_string[2], data_2, plot_2, arg_string[4], data_3, plot_3);
	
	fclose(plot_3);
	fclose(data_3);
	fclose(plot_2);
	fclose(data_2);
	fclose(plot_1);
	fclose(data_1);
	
	return;
}

void cmd_clspins()
{
/*
	short int data_index_parameters=arg_short_int[0];
	int cluster_number=arg_int[0];
	
	static struct run_parameters *parameters;
	static int *parameters_size;

	if (data_input(data, data_parameters, &parameters, &parameters_size, data_index_parameters, data_num)==1) return;

	int nxtimesnytimesnz=(parameters->xlen)*(parameters->ylen)*(parameters->zlen);
	
	int *lattice=(int*)malloc (sizeof(int)*nxtimesnytimesnz);
	struct cluster_stats *cluster_list=(struct cluster_stats*)malloc (sizeof(struct cluster_stats)*nxtimesnytimesnz);
	short int *cluster_spins=(short int*)malloc (sizeof(short int)*nxtimesnytimesnz);

	// Label the clusters.
	clusters (parameters, lattice, parameters->spins);
	
	// Get cluster statistics.
	cluster_stats (parameters, cluster_list, lattice, parameters->spins);
	
	// Get the cluster's spins.
	save_cluster_spins (parameters, cluster_spins, cluster_number, cluster_list, lattice);
	
	FILE *data=fopen (arg_string[0], "w");
	FILE *plot=fopen (arg_string[1], "w");
	
	cluster_spins_plot (cluster_spins, parameters->xlen, parameters->ylen, parameters->zlen, arg_string[0], data, plot);

	fclose (plot);
	fclose (data);
	
	free (cluster_spins);
*/	
	return;
}

void cmd_spins()
{
	short int data_index_parameters=arg_short_int[0];
	short int data_index_flips=arg_short_int[1];
	int plot_steps=arg_int[0];
	
	static struct run_parameters *parameters;
	static int *parameters_size;
	
	// Allocate a block of memory to store parameter values.
	if (data_input(data, data_parameters, &parameters, &parameters_size, data_index_parameters, data_num)==1) return result_error;

	short int *spins=malloc(sizeof(short int)*(parameters->xlen)*(parameters->ylen));
	
	memcpy(spins, parameters->spins_initial, sizeof(short int)*(parameters->xlen)*(parameters->ylen));

	int *flips, *flips_size;

	if (plot_steps>0)
	{
		if (data_input(data, data_parameters, &flips, &flips_size, data_index_flips, data_num)==1) return;
		state(spins, flips, plot_steps);
	}
	
	FILE *data, *plot;
	
	data=fopen(arg_string[0], "w");
	plot=fopen(arg_string[1], "w");

	state_plot_matlab(spins, parameters->xlen, parameters->ylen, plot_steps, arg_string[0], data, plot);
	
	fclose(plot);
	fclose(data);
	
	free(spins);

	return;
}

