/**************************************************************
***************************************************************
*** 
***   cmd-save.c
***
***   Jason Olejarz
***   2011-03-09
***
***   This file contains commands for setting up postprocessing.
***
***************************************************************
**************************************************************/

void cmd_forcesave()
{
	short int save_index=arg_short_int[0];
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
	
	struct save_force_save_data command_data;
	
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
	
	save_force_save(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(save, save_index, save_force_save, save_num);

	return;
}

void cmd_fa()
{
	short int save_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_final_active=arg_short_int[2];
	int num=arg_int[0];
	
	struct save_final_active_data finalactivedata;

	finalactivedata.data_index_parameters=data_index_parameters;
	finalactivedata.data_index_final_active=data_index_final_active;
	finalactivedata.size=num;
	save_final_active(command_insert, &finalactivedata, data, data_parameters, data_num);

	list_insert(save, save_index, save_final_active, save_num);
	
	return;
}

void cmd_fbr()
{
	short int save_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_number_of_blinker_regions=arg_short_int[2];
	short int data_index_volume_of_blinker_regions=arg_short_int[3];
	int num=arg_int[0];
	
	struct save_final_blinkers_data finalblinkersdata;

	finalblinkersdata.data_index_parameters=data_index_parameters;
	finalblinkersdata.data_index_number_of_blinker_regions=data_index_number_of_blinker_regions;
	finalblinkersdata.data_index_volume_of_blinker_regions=data_index_volume_of_blinker_regions;
	finalblinkersdata.size=num;
	save_final_blinkers(command_insert, &finalblinkersdata, data, data_parameters, data_num);
	
	list_insert(save, save_index, save_final_blinkers, save_num);
	
	return;
}

void cmd_fbr2()
{
	short int save_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_number_of_blinker_regions=arg_short_int[2];
	short int data_index_volume_of_blinker_regions=arg_short_int[3];
	short int data_index_blinker_size=arg_short_int[4];
	short int data_index_blinker_perimeter=arg_short_int[5];
	short int data_index_blinker_value_1=arg_short_int[6];
	short int data_index_blinker_value_2=arg_short_int[7];
	short int data_index_blinker_size_index=arg_short_int[8];
	int max_blinkers=arg_int[0];
	short int data_index_spin_type_5=arg_short_int[9];
	int num=arg_int[1];
	
	struct save_final_blinkers2_data finalblinkers2data;

	finalblinkers2data.data_index_parameters=data_index_parameters;
	finalblinkers2data.data_index_number_of_blinker_regions=data_index_number_of_blinker_regions;
	finalblinkers2data.data_index_volume_of_blinker_regions=data_index_volume_of_blinker_regions;
	finalblinkers2data.data_index_blinker_size=data_index_blinker_size;
	finalblinkers2data.data_index_blinker_perimeter=data_index_blinker_perimeter;
	finalblinkers2data.data_index_blinker_value_1=data_index_blinker_value_1;
	finalblinkers2data.data_index_blinker_value_2=data_index_blinker_value_2;
	finalblinkers2data.data_index_blinker_size_index=data_index_blinker_size_index;
	finalblinkers2data.max_blinkers=max_blinkers;
	finalblinkers2data.data_index_spin_type_5=data_index_spin_type_5;
	finalblinkers2data.size=num;
	save_final_blinkers_2(command_insert, &finalblinkers2data, data, data_parameters, data_num);
	
	list_insert(save, save_index, save_final_blinkers_2, save_num);
	
	return;
}

void cmd_fcl()
{
	short int save_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_number_of_clusters=arg_short_int[2];
	short int data_index_cluster_size=arg_short_int[3];
	short int data_index_cluster_sign=arg_short_int[4];
	short int data_index_cluster_size_index=arg_short_int[5];
	int max_clusters=arg_int[0];
	int num=arg_int[1];
	
	struct save_final_clusters_data finalclustersdata;

	finalclustersdata.data_index_parameters=data_index_parameters;
	finalclustersdata.data_index_number_of_clusters=data_index_number_of_clusters;
	finalclustersdata.data_index_cluster_size=data_index_cluster_size;
	finalclustersdata.data_index_cluster_sign=data_index_cluster_sign;
	finalclustersdata.data_index_cluster_size_index=data_index_cluster_size_index;
	finalclustersdata.max_clusters=max_clusters;
	finalclustersdata.size=num;
	save_final_clusters(command_insert, &finalclustersdata, data, data_parameters, data_num);
	
	list_insert(save, save_index, save_final_clusters, save_num);
	
	return;
}

void cmd_fe()
{
	short int save_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_final_energy=arg_short_int[2];
	int num=arg_int[0];
	
	struct save_final_energy_data finalenergydata;

	finalenergydata.data_index_parameters=data_index_parameters;
	finalenergydata.data_index_final_energy=data_index_final_energy;
	finalenergydata.size=num;
	save_final_energy(command_insert, &finalenergydata, data, data_parameters, data_num);

	list_insert(save, save_index, save_final_energy, save_num);
	
	return;
}

void cmd_fm()
{
	short int save_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_final_mag=arg_short_int[2];
	int num=arg_int[0];
	
	struct save_final_mag_data finalmagdata;

	finalmagdata.data_index_parameters=data_index_parameters;
	finalmagdata.data_index_final_mag=data_index_final_mag;
	finalmagdata.size=num;
	save_final_mag(command_insert, &finalmagdata, data, data_parameters, data_num);

	list_insert(save, save_index, save_final_mag, save_num);
	
	return;
}

void cmd_fmabs()
{
	short int save_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_final_mag=arg_short_int[2];
	int num=arg_int[0];
	
	struct save_final_mag_abs_data finalmagabsdata;

	finalmagabsdata.data_index_parameters=data_index_parameters;
	finalmagabsdata.data_index_final_mag=data_index_final_mag;
	finalmagabsdata.size=num;
	save_final_mag_abs(command_insert, &finalmagabsdata, data, data_parameters, data_num);

	list_insert(save, save_index, save_final_mag_abs, save_num);
	
	return;
}

void cmd_fnum0()
{
	short int save_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_final_number_spin=arg_short_int[2];
	int num=arg_int[0];
	
	struct save_final_number_spin_data finalnumberspindata;

	finalnumberspindata.data_index_parameters=data_index_parameters;
	finalnumberspindata.data_index_final_number_spin=data_index_final_number_spin;
	finalnumberspindata.size=num;
	save_final_number_0(command_insert, &finalnumberspindata, data, data_parameters, data_num);

	list_insert(save, save_index, save_final_number_0, save_num);
	
	return;
}

void cmd_fnum1()
{
	short int save_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_final_number_spin=arg_short_int[2];
	int num=arg_int[0];
	
	struct save_final_number_spin_data finalnumberspindata;

	finalnumberspindata.data_index_parameters=data_index_parameters;
	finalnumberspindata.data_index_final_number_spin=data_index_final_number_spin;
	finalnumberspindata.size=num;
	save_final_number_1(command_insert, &finalnumberspindata, data, data_parameters, data_num);

	list_insert(save, save_index, save_final_number_1, save_num);
	
	return;
}

void cmd_fnum2()
{
	short int save_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_final_number_spin=arg_short_int[2];
	int num=arg_int[0];
	
	struct save_final_number_spin_data finalnumberspindata;

	finalnumberspindata.data_index_parameters=data_index_parameters;
	finalnumberspindata.data_index_final_number_spin=data_index_final_number_spin;
	finalnumberspindata.size=num;
	save_final_number_2(command_insert, &finalnumberspindata, data, data_parameters, data_num);

	list_insert(save, save_index, save_final_number_2, save_num);
	
	return;
}

void cmd_fnum3()
{
	short int save_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_final_number_spin=arg_short_int[2];
	int num=arg_int[0];
	
	struct save_final_number_spin_data finalnumberspindata;

	finalnumberspindata.data_index_parameters=data_index_parameters;
	finalnumberspindata.data_index_final_number_spin=data_index_final_number_spin;
	finalnumberspindata.size=num;
	save_final_number_3(command_insert, &finalnumberspindata, data, data_parameters, data_num);

	list_insert(save, save_index, save_final_number_3, save_num);
	
	return;
}

void cmd_fnum4()
{
	short int save_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_final_number_spin=arg_short_int[2];
	int num=arg_int[0];
	
	struct save_final_number_spin_data finalnumberspindata;

	finalnumberspindata.data_index_parameters=data_index_parameters;
	finalnumberspindata.data_index_final_number_spin=data_index_final_number_spin;
	finalnumberspindata.size=num;
	save_final_number_4(command_insert, &finalnumberspindata, data, data_parameters, data_num);

	list_insert(save, save_index, save_final_number_4, save_num);
	
	return;
}

void cmd_fnum5()
{
	short int save_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_final_number_spin=arg_short_int[2];
	int num=arg_int[0];
	
	struct save_final_number_spin_data finalnumberspindata;

	finalnumberspindata.data_index_parameters=data_index_parameters;
	finalnumberspindata.data_index_final_number_spin=data_index_final_number_spin;
	finalnumberspindata.size=num;
	save_final_number_5(command_insert, &finalnumberspindata, data, data_parameters, data_num);

	list_insert(save, save_index, save_final_number_5, save_num);
	
	return;
}

void cmd_fnum6()
{
	short int save_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_final_number_spin=arg_short_int[2];
	int num=arg_int[0];
	
	struct save_final_number_spin_data finalnumberspindata;

	finalnumberspindata.data_index_parameters=data_index_parameters;
	finalnumberspindata.data_index_final_number_spin=data_index_final_number_spin;
	finalnumberspindata.size=num;
	save_final_number_6(command_insert, &finalnumberspindata, data, data_parameters, data_num);

	list_insert(save, save_index, save_final_number_6, save_num);
	
	return;
}

void cmd_fnum7()
{
	short int save_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_final_number_spin=arg_short_int[2];
	int num=arg_int[0];
	
	struct save_final_number_spin_data finalnumberspindata;

	finalnumberspindata.data_index_parameters=data_index_parameters;
	finalnumberspindata.data_index_final_number_spin=data_index_final_number_spin;
	finalnumberspindata.size=num;
	save_final_number_7(command_insert, &finalnumberspindata, data, data_parameters, data_num);

	list_insert(save, save_index, save_final_number_7, save_num);
	
	return;
}

void cmd_fnum8()
{
	short int save_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_final_number_spin=arg_short_int[2];
	int num=arg_int[0];
	
	struct save_final_number_spin_data finalnumberspindata;

	finalnumberspindata.data_index_parameters=data_index_parameters;
	finalnumberspindata.data_index_final_number_spin=data_index_final_number_spin;
	finalnumberspindata.size=num;
	save_final_number_8(command_insert, &finalnumberspindata, data, data_parameters, data_num);

	list_insert(save, save_index, save_final_number_8, save_num);
	
	return;
}

void cmd_fperc()
{
	short int save_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_number_of_clusters=arg_short_int[2];
	short int data_index_cluster_size=arg_short_int[3];
	short int data_index_cluster_perimeter=arg_short_int[4];
	short int data_index_cluster_value=arg_short_int[5];
	short int data_index_cluster_winding_a=arg_short_int[6];
	short int data_index_cluster_winding_b=arg_short_int[7];
	short int data_index_cluster_size_index=arg_short_int[8];
	int max_clusters=arg_int[0];
	short int data_index_number_of_stripes=arg_short_int[9];
	short int data_index_winding_a=arg_short_int[10];
	short int data_index_winding_b=arg_short_int[11];
	int num=arg_int[1];
	
	struct save_final_clusters_percolation_data finalclusterspercdata;

	finalclusterspercdata.data_index_parameters=data_index_parameters;
	finalclusterspercdata.data_index_number_of_clusters=data_index_number_of_clusters;
	finalclusterspercdata.data_index_cluster_size=data_index_cluster_size;
	finalclusterspercdata.data_index_cluster_perimeter=data_index_cluster_perimeter;
	finalclusterspercdata.data_index_cluster_value=data_index_cluster_value;
	finalclusterspercdata.data_index_cluster_winding_a=data_index_cluster_winding_a;
	finalclusterspercdata.data_index_cluster_winding_b=data_index_cluster_winding_b;
	finalclusterspercdata.data_index_cluster_size_index=data_index_cluster_size_index;
	finalclusterspercdata.max_clusters=max_clusters;
	finalclusterspercdata.data_index_number_of_stripes=data_index_number_of_stripes;
	finalclusterspercdata.data_index_winding_a=data_index_winding_a;
	finalclusterspercdata.data_index_winding_b=data_index_winding_b;
	finalclusterspercdata.size=num;
	save_final_clusters_percolation(command_insert, &finalclusterspercdata, data, data_parameters, data_num);
	
	list_insert(save, save_index, save_final_clusters_percolation, save_num);
	
	return;
}

void cmd_fs()
{
	short int save_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_final_steps=arg_short_int[2];
	int num=arg_int[0];
	
	struct save_final_steps_data finalstepsdata;

	finalstepsdata.data_index_parameters=data_index_parameters;
	finalstepsdata.data_index_final_steps=data_index_final_steps;
	finalstepsdata.size=num;
	save_final_steps(command_insert, &finalstepsdata, data, data_parameters, data_num);

	list_insert(save, save_index, save_final_steps, save_num);
	
	return;
}

void cmd_ft()
{
	short int save_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_final_time=arg_short_int[2];
	int num=arg_int[0];
	
	struct save_final_time_data finaltimedata;

	finaltimedata.data_index_parameters=data_index_parameters;
	finaltimedata.data_index_final_time=data_index_final_time;
	finaltimedata.size=num;
	save_final_time(command_insert, &finaltimedata, data, data_parameters, data_num);

	list_insert(save, save_index, save_final_time, save_num);
	
	return;
}

void cmd_fTvert()
{
	short int save_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_final_T_vertices=arg_short_int[2];
	int num=arg_int[0];
	
	struct save_final_T_vertices_data finalTverticesdata;

	finalTverticesdata.data_index_parameters=data_index_parameters;
	finalTverticesdata.data_index_final_T_vertices=data_index_final_T_vertices;
	finalTverticesdata.size=num;
	save_final_T_vertices(command_insert, &finalTverticesdata, data, data_parameters, data_num);

	list_insert(save, save_index, save_final_T_vertices, save_num);
	
	return;
}

void cmd_fTvert2()
{
	short int save_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_final_T_vertices=arg_short_int[2];
	int num=arg_int[0];
	
	struct save_final_T_vertices_data finalTverticesdata;

	finalTverticesdata.data_index_parameters=data_index_parameters;
	finalTverticesdata.data_index_final_T_vertices=data_index_final_T_vertices;
	finalTverticesdata.size=num;
	save_final_T_vertices_2(command_insert, &finalTverticesdata, data, data_parameters, data_num);

	list_insert(save, save_index, save_final_T_vertices_2, save_num);
	
	return;
}

void cmd_svseed()
{
	short int save_index=arg_short_int[0];
	short int data_index_rng=arg_short_int[1];
	short int data_index_low=arg_short_int[2];
	short int data_index_high=arg_short_int[3];
	int num=arg_int[0];
	
	struct save_seed_data seeddata;

	seeddata.data_index_rng=data_index_rng;
	seeddata.data_index_low=data_index_low;
	seeddata.data_index_high=data_index_high;
	seeddata.size=num;
	save_seed(command_insert, &seeddata, data, data_parameters, data_num);

	list_insert(save, save_index, save_seed, save_num);
	
	return;
}

