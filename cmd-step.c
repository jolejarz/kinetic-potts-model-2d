/**************************************************************
***************************************************************
*** 
***   cmd-step.c
***
***   Jason Olejarz
***   2011-01-27
***
***   This file contains commands for intermediate processing.
***
***************************************************************
**************************************************************/

void cmd_check()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	double time=arg_double[0];
	
	struct step_check_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.time=time;
	
	step_check(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_check, step_num);

	return;
}

void cmd_check2()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	double time=arg_double[0];
	
	struct step_check_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.time=time;
	
	step_check_2(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_check_2, step_num);

	return;
}

void cmd_energy()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	short int data_index_energy=arg_short_int[3];
	
	struct step_energy_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.data_index_energy=data_index_energy;
	
	step_energy(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_energy, step_num);

	return;
}

void cmd_energy2()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_energy=arg_short_int[2];
	unsigned long int starting_step=arg_unsigned_long_int[0];
	unsigned long int ending_step=arg_unsigned_long_int[1];
	
	struct step_energy_2_data energy2data;
	
	energy2data.data_index_parameters=data_index_parameters;
	energy2data.data_index_energy=data_index_energy;
	energy2data.starting_step=starting_step;
	energy2data.ending_step=ending_step;
	step_energy_2(command_insert, &energy2data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_energy_2, step_num);
	
	return;
}

void cmd_energyavg()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_energy=arg_short_int[2];
	short int data_index_runs=arg_short_int[3];
	double starting_time=arg_double[0];
	double ending_time=arg_double[1];
	double time_interval=arg_double[2];
	
	struct step_energy_average_data energyavgdata;
	
	energyavgdata.data_index_parameters=data_index_parameters;
	energyavgdata.data_index_energy=data_index_energy;
	energyavgdata.data_index_runs=data_index_runs;
	energyavgdata.starting_time=starting_time;
	energyavgdata.ending_time=ending_time;
	energyavgdata.time_interval=time_interval;
	step_energy_average(command_insert, &energyavgdata, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_energy_average, step_num);
	
	return;
}

void cmd_Tvertavg()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_T_vertices=arg_short_int[2];
	short int data_index_runs=arg_short_int[3];
	double starting_time=arg_double[0];
	double ending_time=arg_double[1];
	double time_interval=arg_double[2];
	
	struct step_T_vertices_average_data Tvertavgdata;
	
	Tvertavgdata.data_index_parameters=data_index_parameters;
	Tvertavgdata.data_index_T_vertices=data_index_T_vertices;
	Tvertavgdata.data_index_runs=data_index_runs;
	Tvertavgdata.starting_time=starting_time;
	Tvertavgdata.ending_time=ending_time;
	Tvertavgdata.time_interval=time_interval;
	step_T_vertices_average(command_insert, &Tvertavgdata, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_T_vertices_average, step_num);
	
	return;
}

void cmd_Tvert2avg()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_T_vertices=arg_short_int[2];
	short int data_index_runs=arg_short_int[3];
	double starting_time=arg_double[0];
	double ending_time=arg_double[1];
	double time_interval=arg_double[2];
	
	struct step_T_vertices_average_data Tvertavgdata;
	
	Tvertavgdata.data_index_parameters=data_index_parameters;
	Tvertavgdata.data_index_T_vertices=data_index_T_vertices;
	Tvertavgdata.data_index_runs=data_index_runs;
	Tvertavgdata.starting_time=starting_time;
	Tvertavgdata.ending_time=ending_time;
	Tvertavgdata.time_interval=time_interval;
	step_T_vertices_2_average(command_insert, &Tvertavgdata, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_T_vertices_2_average, step_num);
	
	return;
}

void cmd_flips()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_flips=arg_short_int[2];
	unsigned long int steps=arg_unsigned_long_int[0];
	
	struct step_flips_data flipsdata;
	
	flipsdata.data_index_parameters=data_index_parameters;
	flipsdata.data_index_flips=data_index_flips;
	flipsdata.steps=steps;
	step_flips(command_insert, &flipsdata, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_flips, step_num);
	
	return;
}

void cmd_flips2()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_flips=arg_short_int[2];
	unsigned long int starting_step=arg_unsigned_long_int[0];
	unsigned long int ending_step=arg_unsigned_long_int[1];
	
	struct step_flips_2_data flips2data;
	
	flips2data.data_index_parameters=data_index_parameters;
	flips2data.data_index_flips=data_index_flips;
	flips2data.starting_step=starting_step;
	flips2data.ending_step=ending_step;
	step_flips_2(command_insert, &flips2data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_flips_2, step_num);
	
	return;
}

void cmd_flipsb2()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_flips_backward=arg_short_int[2];
	unsigned long int starting_step=arg_unsigned_long_int[0];
	unsigned long int ending_step=arg_unsigned_long_int[1];
	
	struct step_flips_backward_2_data flipsbackward2data;
	
	flipsbackward2data.data_index_parameters=data_index_parameters;
	flipsbackward2data.data_index_flips_backward=data_index_flips_backward;
	flipsbackward2data.starting_step=starting_step;
	flipsbackward2data.ending_step=ending_step;
	step_flips_backward_2(command_insert, &flipsbackward2data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_flips_backward_2, step_num);
	
	return;
}

void cmd_force()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	double time=arg_double[0];
	double additional_time=arg_double[1];
	
	struct step_force_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.time=time;
	command_data.additional_time=additional_time;
	
	step_force(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_force, step_num);

	return;
}

void cmd_force2()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	double time=arg_double[0];
	double additional_time=arg_double[1];
	short int data_index_energy_0=arg_short_int[2];
	short int data_index_energy_1=arg_short_int[3];
	short int data_index_energy_2=arg_short_int[4];
	short int data_index_energy_diff_1=arg_short_int[5];
	short int data_index_energy_diff_2=arg_short_int[6];
	short int data_index_num_energy_drops_1=arg_short_int[7];
	short int data_index_num_energy_drops_2=arg_short_int[8];
	int num=arg_int[0];
	
	struct step_force2_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.time=time;
	command_data.additional_time=additional_time;
	command_data.data_index_energy_0=data_index_energy_0;
	command_data.data_index_energy_1=data_index_energy_1;
	command_data.data_index_energy_2=data_index_energy_2;
	command_data.data_index_energy_diff_1=data_index_energy_diff_1;
	command_data.data_index_energy_diff_2=data_index_energy_diff_2;
	command_data.data_index_num_energy_drops_1=data_index_num_energy_drops_1;
	command_data.data_index_num_energy_drops_2=data_index_num_energy_drops_2;
	command_data.size=num;
	
	step_force_2(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_force_2, step_num);

	return;
}

void cmd_mag()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	short int data_index_mag=arg_short_int[3];
	
	struct step_mag_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.data_index_mag=data_index_mag;
	
	step_mag(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_mag, step_num);

	return;
}

void cmd_steplat()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	unsigned long int plot_step=arg_unsigned_long_int[0];
	short int data_index_spins=arg_short_int[2];
	char *matlab_data_filename=arg_string[0];
	char *matlab_plot_filename=arg_string[1];
	
	struct step_save_lattice_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.plot_step=plot_step;
	command_data.data_index_spins=data_index_spins;
	command_data.matlab_data_filename=matlab_data_filename;
	command_data.matlab_plot_filename=matlab_plot_filename;

	step_save_lattice(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_save_lattice, step_num);

	return;
}

void cmd_Tvert()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	short int data_index_T_vertices=arg_short_int[3];
	
	struct step_T_vertices_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.data_index_T_vertices=data_index_T_vertices;
	
	step_T_vertices(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_T_vertices, step_num);

	return;
}

void cmd_Tvert2()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	short int data_index_T_vertices=arg_short_int[3];
	
	struct step_T_vertices_2_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.data_index_T_vertices=data_index_T_vertices;
	
	step_T_vertices_2(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_T_vertices_2, step_num);

	return;
}

void cmd_time()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	short int data_index_time=arg_short_int[3];
	
	struct step_time_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.data_index_time=data_index_time;
	
	step_time(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_time, step_num);

	return;

}

void cmd_time2()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_time=arg_short_int[2];
	unsigned long int starting_step=arg_unsigned_long_int[0];
	unsigned long int ending_step=arg_unsigned_long_int[1];
	
	struct step_time_2_data time2data;
	
	time2data.data_index_parameters=data_index_parameters;
	time2data.data_index_time=data_index_time;
	time2data.starting_step=starting_step;
	time2data.ending_step=ending_step;
	step_time_2(command_insert, &time2data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_time_2, step_num);
	
	return;
}

void cmd_trigs()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	unsigned long int steps_interval=arg_unsigned_long_int[0];
	short int progress=arg_short_int[3];
	
	struct step_trigger_steps_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.steps_interval=steps_interval;
	command_data.progress=progress;

	step_trigger_steps(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_trigger_steps, step_num);
	
	return;
}

void cmd_trigt()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	double time_interval=arg_double[0];
	short int progress=arg_short_int[3];
	
	struct step_trigger_time_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.time_interval=time_interval;
	command_data.progress=progress;

	step_trigger_time(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_trigger_time, step_num);

	return;
}

void cmd_trigtonce()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	double time=arg_double[0];
	int runs=arg_int[0];
	
	struct step_trigger_time_once_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.time=time;
	command_data.runs=runs;

	step_trigger_time_once(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_trigger_time_once, step_num);
	
	return;
}

