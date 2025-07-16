/**************************************************************
***************************************************************
*** 
***   save.h
***
***   Jason Olejarz
***   2010-07-21
***
***   This is the header file for save.c.
***
***************************************************************
**************************************************************/

struct save_force_save_data
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
struct save_force_save_parameters
{
	int size;
	int index;
};

struct save_final_active_data
{
	short int data_index_parameters;
	short int data_index_final_active;
	int size;
};
struct save_final_active_parameters
{
	int size;
	int index;
};

struct save_final_blinkers_data
{
	short int data_index_parameters;
	short int data_index_number_of_blinker_regions;
	short int data_index_volume_of_blinker_regions;
	int size;
};
struct save_final_blinkers_parameters
{
	int size;
	int index;
};

struct save_final_blinkers2_data
{
	short int data_index_parameters;
	short int data_index_number_of_blinker_regions;
	short int data_index_volume_of_blinker_regions;
	short int data_index_blinker_size;
	short int data_index_blinker_perimeter;
	short int data_index_blinker_value_1;
	short int data_index_blinker_value_2;
	short int data_index_blinker_size_index;
	int max_blinkers;
	short int data_index_spin_type_5;
	int size;
};
struct save_final_blinkers2_parameters
{
	int size;
	int index;
};

struct save_final_clusters_data
{
	short int data_index_parameters;
	short int data_index_number_of_clusters;
	short int data_index_cluster_size;
	short int data_index_cluster_sign;
	short int data_index_cluster_size_index;
	int max_clusters;
	int size;
};
struct save_final_clusters_parameters
{
	int size;
	int index;
};

struct save_final_clusters_percolation_data
{
	short int data_index_parameters;
	short int data_index_number_of_clusters;
	short int data_index_cluster_size;
	short int data_index_cluster_perimeter;
	short int data_index_cluster_value;
	short int data_index_cluster_winding_a;
	short int data_index_cluster_winding_b;
	short int data_index_cluster_size_index;
	int max_clusters;
	short int data_index_number_of_stripes;
	short int data_index_winding_a;
	short int data_index_winding_b;
	int size;
};
struct save_final_clusters_percolation_parameters
{
	int size;
	int index;
};

struct save_final_energy_data
{
	short int data_index_parameters;
	short int data_index_final_energy;
	int size;
};
struct save_final_energy_parameters
{
	int size;
	int index;
};

struct save_final_mag_data
{
	short int data_index_parameters;
	short int data_index_final_mag;
	int size;
};
struct save_final_mag_parameters
{
	int size;
	int index;
};

struct save_final_mag_abs_data
{
	short int data_index_parameters;
	short int data_index_final_mag;
	int size;
};
struct save_final_mag_abs_parameters
{
	int size;
	int index;
};

struct save_final_number_spin_data
{
	short int data_index_parameters;
	short int data_index_final_number_spin;
	int size;
};
struct save_final_number_spin_parameters
{
	int size;
	int index;
};

struct save_final_steps_data
{
	short int data_index_parameters;
	short int data_index_final_steps;
	int size;
};
struct save_final_steps_parameters
{
	int size;
	int index;
};

struct save_final_time_data
{
	short int data_index_parameters;
	short int data_index_final_time;
	int size;
};
struct save_final_time_parameters
{
	int size;
	int index;
};

struct save_final_T_vertices_data
{
	short int data_index_parameters;
	short int data_index_final_T_vertices;
	int size;
};
struct save_final_T_vertices_parameters
{
	int size;
	int index;
};

struct save_seed_data
{
	short int data_index_rng;
	short int data_index_low;
	short int data_index_high;
	int size;
};
struct save_seed_parameters
{
	int size;
	int index;
};

struct save_value_data
{
	short int data_index;
	int size;
};
struct save_value_parameters
{
	int size;
	int index;
};

short int save_force_save(short int command, struct save_force_save_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int save_final_active(short int command, struct save_final_active_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int save_final_blinkers(short int command, struct save_final_blinkers_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int save_final_blinkers_2(short int command, struct save_final_blinkers2_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int save_final_clusters(short int command, struct save_final_clusters_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int save_final_clusters_percolation(short int command, struct save_final_clusters_percolation_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int save_final_energy(short int command, struct save_final_energy_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int save_final_mag(short int command, struct save_final_mag_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int save_final_mag_abs(short int command, struct save_final_mag_abs_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int save_final_number_0(short int command, struct save_final_number_spin_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int save_final_number_1(short int command, struct save_final_number_spin_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int save_final_number_2(short int command, struct save_final_number_spin_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int save_final_number_3(short int command, struct save_final_number_spin_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int save_final_number_4(short int command, struct save_final_number_spin_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int save_final_number_5(short int command, struct save_final_number_spin_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int save_final_number_6(short int command, struct save_final_number_spin_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int save_final_number_7(short int command, struct save_final_number_spin_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int save_final_number_8(short int command, struct save_final_number_spin_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int save_final_steps(short int command, struct save_final_steps_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int save_final_time(short int command, struct save_final_time_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int save_final_T_vertices(short int command, struct save_final_T_vertices_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int save_final_T_vertices_2(short int command, struct save_final_T_vertices_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int save_seed(short int command, struct save_seed_data *command_data, void *data[], void *data_parameters[], short int data_size);

