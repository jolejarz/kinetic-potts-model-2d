/**************************************************************
***************************************************************
*** 
***   step.h
***
***   Jason Olejarz
***   2011-01-27
***
***   This is the header file for step.c.
***
***************************************************************
**************************************************************/

struct step_check_data
{
	short int data_index_parameters;
	double time;
};
struct step_check_parameters
{
};

struct step_energy_data
{
	short int data_index_parameters;
	short int data_index_trigger;
	short int data_index_energy;
};
struct step_energy_parameters
{
	int size;
	int index;
};

struct step_energy_2_data
{
	short int data_index_parameters;
	short int data_index_energy;
	unsigned long int starting_step;
	unsigned long int ending_step;
};
struct step_energy_2_parameters
{
	int size;
	int index;
};

struct step_energy_average_data
{
	short int data_index_parameters;
	short int data_index_energy;
	short int data_index_runs;
	double starting_time;
	double ending_time;
	double time_interval;
};
struct step_energy_average_parameters
{
	int size;
	int index;
};

struct step_T_vertices_average_data
{
	short int data_index_parameters;
	short int data_index_T_vertices;
	short int data_index_runs;
	double starting_time;
	double ending_time;
	double time_interval;
};
struct step_T_vertices_average_parameters
{
	int size;
	int index;
};

struct step_flips_data
{
	short int data_index_parameters;
	short int data_index_flips;
	int steps;
};
struct step_flips_parameters
{
	int size;
	int index;
};

struct step_flips_2_data
{
	short int data_index_parameters;
	short int data_index_flips;
	unsigned long int starting_step;
	unsigned long int ending_step;
};
struct step_flips_2_parameters
{
	int size;
	int index;
};

struct step_flips_backward_2_data
{
	short int data_index_parameters;
	short int data_index_flips_backward;
	unsigned long int starting_step;
	unsigned long int ending_step;
};
struct step_flips_backward_2_parameters
{
	int size;
	int index;
};

struct step_force_data
{
	short int data_index_parameters;
	double time;
	double additional_time;
};
struct step_force_parameters
{
};

struct step_force2_data
{
	short int data_index_parameters;
	double time;
	double additional_time;
	short int data_index_energy_0;
	short int data_index_energy_1;
	short int data_index_energy_2;
	short int data_index_energy_diff_1;
	short int data_index_energy_diff_2;
	short int data_index_num_energy_drops_1;
	short int data_index_num_energy_drops_2;
	int size;
};
struct step_force2_parameters
{
	int size;
	int index;
};

struct step_mag_data
{
	short int data_index_parameters;
	short int data_index_trigger;
	short int data_index_mag;
};
struct step_mag_parameters
{
	int size;
	int index;
};

struct step_save_lattice_data
{
	short int data_index_parameters;
	unsigned long int plot_step;
	short int data_index_spins;
	char *matlab_data_filename;
	char *matlab_plot_filename;
};
struct step_save_lattice_parameters
{
};

struct step_T_vertices_data
{
	short int data_index_parameters;
	short int data_index_trigger;
	short int data_index_T_vertices;
};
struct step_T_vertices_parameters
{
	int size;
	int index;
};

struct step_T_vertices_2_data
{
	short int data_index_parameters;
	short int data_index_trigger;
	short int data_index_T_vertices;
};
struct step_T_vertices_2_parameters
{
	int size;
	int index;
};

struct step_time_data
{
	short int data_index_parameters;
	short int data_index_trigger;
	short int data_index_time;
};
struct step_time_parameters
{
	int size;
	int index;
};

struct step_time_2_data
{
	short int data_index_parameters;
	short int data_index_time;
	unsigned long int starting_step;
	unsigned long int ending_step;
};
struct step_time_2_parameters
{
	int size;
	int index;
};

struct step_trigger_steps_data
{
	short int data_index_parameters;
	short int data_index_trigger;
	unsigned long int steps_interval;
	short int progress;
};
struct step_trigger_time_data
{
	short int data_index_parameters;
	short int data_index_trigger;
	double time_interval;
	short int progress;
};
struct step_trigger_time_once_data
{
	short int data_index_parameters;
	short int data_index_trigger;
	double time;
	int runs;
};
// The parameters data index for a trigger is the same whether the trigger is after a number of steps
// have elapsed or after an amount of time has elapsed.
struct step_trigger_parameters
{
	int total;
	int current;
	unsigned long int steps;
	double time;
};

short int step_check(short int command, struct step_check_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_check_2(short int command, struct step_check_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_energy(short int command, struct step_energy_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_energy_2(short int command, struct step_energy_2_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_energy_average(short int command, struct step_energy_average_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_T_vertices_average(short int command, struct step_T_vertices_average_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_T_vertices_2_average(short int command, struct step_T_vertices_average_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_flips(short int command, struct step_flips_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_flips_2(short int command, struct step_flips_2_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_flips_backward_2(short int command, struct step_flips_backward_2_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_force(short int command, struct step_force_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_force_2(short int command, struct step_force2_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_mag(short int command, struct step_mag_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_save_lattice(short int command, struct step_save_lattice_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_T_vertices(short int command, struct step_T_vertices_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_T_vertices_2(short int command, struct step_T_vertices_2_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_time(short int command, struct step_time_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_time_2(short int command, struct step_time_2_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_trigger_steps(short int command, struct step_trigger_steps_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_trigger_time(short int command, struct step_trigger_time_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_trigger_time_once(short int command, struct step_trigger_time_once_data *command_data, void *data[], void *data_parameters[], short int data_size);
