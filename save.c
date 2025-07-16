/**************************************************************
***************************************************************
*** 
***   save.c
***
***   Jason Olejarz
***   2010-07-21
***
***   This file contains the routines used by 'calc' to save data after each run of 'evolve'.
***
***************************************************************
**************************************************************/

#include "includes.h"

short int save_force_save(short int command, struct save_force_save_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct save_force_save_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;

	static double prob_backup[11+MAX_Q*(MAX_Q-1)];

	static int last_energy;

	static short int same_energy_count;

	static short int bias;

	static short int q;

	static short int eligible_spins;
	
	static short int run_force;

	static int i, j, k;

	static double energy_0_value, energy_1_value, energy_2_value;
	static double energy_diff_1_value, energy_diff_2_value;
	static int num_energy_drops_1_value, num_energy_drops_2_value;

	static double *energy_0;
	static struct step_force2_parameters *energy_0_parameters;

	static double *energy_1;
	static struct step_force2_parameters *energy_1_parameters;

	static double *energy_2;
	static struct step_force2_parameters *energy_2_parameters;

	static double *energy_diff_1;
	static struct step_force2_parameters *energy_diff_1_parameters;

	static double *energy_diff_2;
	static struct step_force2_parameters *energy_diff_2_parameters;

	static double *num_energy_drops_1;
	static struct step_force2_parameters *num_energy_drops_1_parameters;

	static double *num_energy_drops_2;
	static struct step_force2_parameters *num_energy_drops_2_parameters;

	switch (command)
	{
		case command_info: {printf("save_force_save\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct save_force_save_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			
			if (data_output(data, data_parameters, &energy_0, &energy_0_parameters, cmd_data.size*sizeof(double), sizeof(struct step_force2_parameters), cmd_data.data_index_energy_0, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &energy_1, &energy_1_parameters, cmd_data.size*sizeof(double), sizeof(struct step_force2_parameters), cmd_data.data_index_energy_1, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &energy_2, &energy_2_parameters, cmd_data.size*sizeof(double), sizeof(struct step_force2_parameters), cmd_data.data_index_energy_2, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &energy_diff_1, &energy_diff_1_parameters, cmd_data.size*sizeof(double), sizeof(struct step_force2_parameters), cmd_data.data_index_energy_diff_1, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &energy_diff_2, &energy_diff_2_parameters, cmd_data.size*sizeof(double), sizeof(struct step_force2_parameters), cmd_data.data_index_energy_diff_2, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &num_energy_drops_1, &num_energy_drops_1_parameters, cmd_data.size*sizeof(double), sizeof(struct step_force2_parameters), cmd_data.data_index_num_energy_drops_1, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &num_energy_drops_2, &num_energy_drops_2_parameters, cmd_data.size*sizeof(double), sizeof(struct step_force2_parameters), cmd_data.data_index_num_energy_drops_2, data_size)==1) return result_error;

			energy_0_parameters->index=0;
			energy_0_parameters->size=0;

			energy_1_parameters->index=0;
			energy_1_parameters->size=0;

			energy_2_parameters->index=0;
			energy_2_parameters->size=0;

			energy_diff_1_parameters->index=0;
			energy_diff_1_parameters->size=0;

			energy_diff_2_parameters->index=0;
			energy_diff_2_parameters->size=0;

			num_energy_drops_1_parameters->index=0;
			num_energy_drops_1_parameters->size=0;

			num_energy_drops_2_parameters->index=0;
			num_energy_drops_2_parameters->size=0;

			q=p->q;

			return result_continue;
		}
		case command_begin:
		{
			return result_continue;
		}
		case command_run:
		{
			// Initialize these variables.
			energy_0_value=p->energy;
			energy_1_value=0;
			num_energy_drops_1_value=0;
			num_energy_drops_2_value=0;

			num_energy_drops_1_value=p->num_energy_drops;

//			if (p->time>=cmd_data.time)
//			{
				energy_1_value=p->energy;

				// Backup the original flipping probabilities.
				for (i=0; i<11+q*(q-1); i++)
				{
					prob_backup[i]=p->prob[i];
				}

				last_energy=(int)(p->energy);

				same_energy_count=0;

				bias=0;

				while (1)
				{
					run_force=0;

					eligible_spins=0;

					for (i=0; i<q*(q-1); i++)
					{
						p->prob[11+i]=0;
					}

					if (bias%2==0)
					{
						for (i=0; i<q*(q-1); i++)
						{
							j=i/(q-1);
							k=((i%(q-1))+j+1)%q;

							if (j!=k && k==bias/2)
							{
								p->prob[11+i]=0.5;

								if (*(p->index[11+i])>0) eligible_spins++;
							}
						}

						if (eligible_spins>0) run_force=1;
					}
					else
					{
						for (i=0; i<q-1; i++)
						{
							p->prob[11+(bias/2)*(q-1)+i]=0.5;

							if (*(p->index[11+(bias/2)*(q-1)+i])>0) eligible_spins++;
						}

						if (eligible_spins>0) run_force=1;
					}

					// Check if there are eligible spins.
					if (run_force==1)
					{
						// Drive the system with a bias field.
						if (run(	0,
								0,
								data,
								data_parameters,
								data_size,
								p,
								0)
							==result_error) return result_error;
					}

					num_energy_drops_2_value+=p->num_energy_drops;

					if (last_energy-(int)(p->energy)>0)
					{
						last_energy=(int)(p->energy);
						same_energy_count=0;
					}
					else same_energy_count++;

					if (same_energy_count==2*q)
					{
						if (cmd_data.additional_time>0)
						{
							// Allow the system to evolve a little longer so the
							// blinker regions can fill out.

							eligible_spins=0;
							for (i=3; i<11+q*(q-1); i++)
							{
								if (*(p->index[i])>0) eligible_spins++;
							}

							if (eligible_spins>0)
							{
								for (i=0; i<q*(q-1); i++)
								{
									p->prob[11+i]=0.5;
								}
								
								p->maxtime=p->time+cmd_data.additional_time;

								if (run(	0,
										0,
										data,
										data_parameters,
										data_size,
										p,
										1)
									==result_error) return result_error;
							}
						}

						// Restore the original flipping probabilities.
						for (i=0; i<11+q*(q-1); i++)
						{
							p->prob[i]=prob_backup[i];
						}

//						return result_done;

						energy_2_value=p->energy;
						if (energy_2_value>energy_1_value) energy_1_value=energy_2_value;

						energy_diff_1_value=energy_0_value-energy_1_value;
						energy_diff_2_value=energy_1_value-energy_2_value;

						*(energy_0+(energy_0_parameters->index))=energy_0_value;

						(energy_0_parameters->index)++;
						(energy_0_parameters->size)+=sizeof(double);

						*(energy_1+(energy_1_parameters->index))=energy_1_value;

						(energy_1_parameters->index)++;
						(energy_1_parameters->size)+=sizeof(double);

						*(energy_2+(energy_2_parameters->index))=energy_2_value;

						(energy_2_parameters->index)++;
						(energy_2_parameters->size)+=sizeof(double);

						*(energy_diff_1+(energy_diff_1_parameters->index))=energy_diff_1_value;

						(energy_diff_1_parameters->index)++;
						(energy_diff_1_parameters->size)+=sizeof(double);

						*(energy_diff_2+(energy_diff_2_parameters->index))=energy_diff_2_value;

						(energy_diff_2_parameters->index)++;
						(energy_diff_2_parameters->size)+=sizeof(double);

						*(num_energy_drops_1+(num_energy_drops_1_parameters->index))=(double)num_energy_drops_1_value;

						(num_energy_drops_1_parameters->index)++;
						(num_energy_drops_1_parameters->size)+=sizeof(double);

						*(num_energy_drops_2+(num_energy_drops_2_parameters->index))=(double)num_energy_drops_2_value;

						(num_energy_drops_2_parameters->index)++;
						(num_energy_drops_2_parameters->size)+=sizeof(double);

						return result_continue;
					}

					// Change the bias field.
					if (bias<2*q-1) bias++; else bias=0;
				}
//			}
			return result_continue;
		}
		case command_end:
		{
			return result_continue;
		}
		default: return result_continue;
	}
}

short int save_final_active(short int command, struct save_final_active_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct save_final_active_data cmd_data;
	
	static double *final_active;

	static struct run_parameters *parameters;
	static int *parameters_size;
	
	static struct save_final_active_parameters *final_active_parameters;

	static int q;

	static int i;

	switch (command)
	{
		case command_info: {printf("save_final_active\n"); return result_continue;}
		case command_insert:
		{
			memcpy(&cmd_data, command_data, sizeof(struct save_final_active_data));
			
			return result_continue;
		}
		case command_begin:
		{
			if (data_input(data, data_parameters, &parameters, &parameters_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &final_active, &final_active_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_active_parameters), cmd_data.data_index_final_active, data_size)==1) return result_error;
			
			final_active_parameters->index=0;
			final_active_parameters->size=0;

			q=parameters->q;
			
			return result_continue;
		}
		case command_run:
		{
			*(final_active+(final_active_parameters->index))=0;
			for (i=0; i<q*(q-1); i++)
			{
				*(final_active+(final_active_parameters->index))+=(double)(*(parameters->index[11+i]));
			}
			
			(final_active_parameters->index)++;
			final_active_parameters->size+=sizeof(double);

			return result_continue;
		}
		default: return result_continue;
	}
}

short int save_final_blinkers(short int command, struct save_final_blinkers_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct save_final_blinkers_data cmd_data;

	static short int data_index_parameters;
	static short int data_index_number_of_blinker_regions;
	static short int data_index_volume_of_blinker_regions;
	
	static double *number_of_blinker_regions;
	static struct save_final_blinkers_parameters *number_of_blinker_regions_parameters;

	static double *volume_of_blinker_regions;
	static struct save_final_blinkers_parameters *volume_of_blinker_regions_parameters;

	static struct run_parameters *p;
	static int *p_size;
	
	static int i, j, k, count, nxtimesny;

	static short int q;

	static int number_of_clusters;
	
	static int *lattice;
	static struct cluster_stats *cluster_list;
	static short int *cluster_spins;
	
	static unsigned long int rng_seed_backup;
	static unsigned long int rng_long_int_backup;
	static double rng_double_backup;

	static short int *spins_backup;
	static short int *state_backup;
	static int *map_backup[11+MAX_Q*(MAX_Q-1)];
	static int *index_backup[11+MAX_Q*(MAX_Q-1)];
	static double prob_backup[11+MAX_Q*(MAX_Q-1)];
	static int coordinate_backup;
	static short int move_backup;
	static unsigned long int steps_backup;
	static double time_backup;
	static double energy_backup;
	static int mag_backup;
	static int num_value_backup[MAX_Q];
	static short int *T_vertices_backup;
	static short int *T_vertices_2_backup;

	static short int *spins_plus_field;
	static short int *spins_minus_field;

	static short int *blinker_regions;

	static short int bias;

	static short int eligible_spins;

	static short int run_force;

	static int blinker_regions_count;
	static int total_blinker_size;

	switch (command)
	{
		case command_info: {printf("save_final_blinkers\n"); return result_continue;}
		case command_insert:
		{
			memcpy(&cmd_data, command_data, sizeof(struct save_final_blinkers_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;

			nxtimesny=(p->xlen)*(p->ylen);

			q=p->q;

			lattice=(int*)malloc (sizeof(int)*nxtimesny);
			cluster_list=(struct cluster_stats*)malloc (sizeof(struct cluster_stats)*nxtimesny);
			cluster_spins=(short int*)malloc (sizeof(short int)*nxtimesny);
			
			// Allocate memory.
			spins_backup=(short int*)malloc (sizeof(short int)*(p->xlen)*(p->ylen));
			state_backup=(short int*)malloc (sizeof(short int)*(p->xlen)*(p->ylen));
			for (i=0; i<11+q*(q-1); i++)
			{
				map_backup[i]=(int*)malloc (sizeof(int)*(p->xlen)*(p->ylen));
				index_backup[i]=(int*)malloc (sizeof(int)*(1+(p->xlen)*(p->ylen)));
			}
			T_vertices_backup=(short int*)malloc (sizeof(short int)*(p->xlen)*(p->ylen));
			T_vertices_2_backup=(short int*)malloc (sizeof(short int)*(p->xlen)*(p->ylen));

			// These are used for storing the lattice after application of plus and minus bias fields.
			spins_plus_field=(short int*)malloc (sizeof(short int)*(p->xlen)*(p->ylen));
			spins_minus_field=(short int*)malloc (sizeof(short int)*(p->xlen)*(p->ylen));

			// This is used for saving the blinker regions.
			// A `0' value means that the lattice site is not part of a blinker region.
			// A `1' value means that the lattice site is part of a blinker region.
			blinker_regions=(short int*)malloc (sizeof(short int)*(p->xlen)*(p->ylen));

			return result_continue;
		}
		case command_begin:
		{
			if ((p->xlen)*(p->ylen)!=nxtimesny)
			{
				nxtimesny=(p->xlen)*(p->ylen);

				lattice=(int*)realloc (lattice, sizeof(int)*nxtimesny);
				cluster_list=(struct cluster_stats*)realloc (cluster_list, sizeof(struct cluster_stats)*nxtimesny);
				cluster_spins=(short int*)realloc (cluster_spins, sizeof(short int)*nxtimesny);
			}
			
			if (data_output(data, data_parameters, &number_of_blinker_regions, &number_of_blinker_regions_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_blinkers_parameters), cmd_data.data_index_number_of_blinker_regions, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &volume_of_blinker_regions, &volume_of_blinker_regions_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_blinkers_parameters), cmd_data.data_index_volume_of_blinker_regions, data_size)==1) return result_error;
			
			number_of_blinker_regions_parameters->index=0;
			number_of_blinker_regions_parameters->size=0;
			
			volume_of_blinker_regions_parameters->index=0;
			volume_of_blinker_regions_parameters->size=0;
			
			return result_continue;
		}
		case command_delete:
		{
			free (cluster_spins);
			free (cluster_list);
			free (lattice);
			return result_continue;
		}
		case command_run:
		{
			// Backup all simulation data.
			memcpy (spins_backup, p->spins, sizeof(short int)*(p->xlen)*(p->ylen));
			memcpy (state_backup, p->state, sizeof(short int)*(p->xlen)*(p->ylen));
			for (i=0; i<11+q*(q-1); i++)
			{
				memcpy (map_backup[i], p->map[i], sizeof(int)*(p->xlen)*(p->ylen));
				memcpy (index_backup[i], p->index[i], sizeof(int)*(1+(p->xlen)*(p->ylen)));
				prob_backup[i]=p->prob[i];
			}
			coordinate_backup=p->coordinate;
			move_backup=p->move;
			steps_backup=p->steps;
			time_backup=p->time;
			energy_backup=p->energy;
			mag_backup=p->mag;
			for (i=0; i<q; i++)
			{
				num_value_backup[i]=p->num_value[i];
			}
			memcpy (T_vertices_backup, p->T_vertices, sizeof(short int)*(p->xlen)*(p->ylen));
			memcpy (T_vertices_2_backup, p->T_vertices_2, sizeof(short int)*(p->xlen)*(p->ylen));

			// Initialize this to `0'.
			memset (blinker_regions, 0, sizeof(short int)*(p->xlen)*(p->ylen));

			for (bias=0; bias<2*q; bias++)
			{
				run_force=0;

				eligible_spins=0;

				for (i=0; i<q*(q-1); i++)
				{
					p->prob[11+i]=0;
				}

				if (bias%2==0)
				{
					for (i=0; i<q*(q-1); i++)
					{
						j=i/(q-1);
						k=((i%(q-1))+j+1)%q;

						if (j!=k && k==bias/2)
						{
							p->prob[11+i]=0.5;

							if (*(p->index[11+i])>0) eligible_spins++;
						}
					}

					if (eligible_spins>0) run_force=1;
				}
				else
				{
					for (i=0; i<q-1; i++)
					{
						p->prob[11+(bias/2)*(q-1)+i]=0.5;

						if (*(p->index[11+(bias/2)*(q-1)+i])>0) eligible_spins++;
					}

					if (eligible_spins>0) run_force=1;
				}

				// Check if there are eligible spins.
				if (run_force==1)
				{
					// Backup the state of the random number generator.
					rng_seed_backup=(p->rng_struct)->seed;
					rng_long_int_backup=(p->rng_struct)->current_long_int;
					rng_double_backup=(p->rng_struct)->current_double;
					(p->rng_struct)->rng_backup();
					
					// Drive the system with a bias field.
					if (run(	0,
							0,
							0,
							0,
							0,
							p,
							0)
						==result_error) return result_error;

					// Restore the state of the random number generator.
					(p->rng_struct)->seed=rng_seed_backup;
					(p->rng_struct)->current_long_int=rng_long_int_backup;
					(p->rng_struct)->current_double=rng_double_backup;
					(p->rng_struct)->rng_restore();
				}

				if (bias%2==0)
				{
					memcpy (spins_plus_field, p->spins, sizeof(short int)*(p->xlen)*(p->ylen));
				}
				else
				{
					memcpy (spins_minus_field, p->spins, sizeof(short int)*(p->xlen)*(p->ylen));

					for (i=0; i<nxtimesny; i++)
					{
						if (*(spins_plus_field+i)!=*(spins_minus_field+i)) *(blinker_regions+i)=1;
					}
				}
			}
			
			// Restore the original lattice.
			memcpy (p->spins, spins_backup, sizeof(short int)*(p->xlen)*(p->ylen));
			memcpy (p->state, state_backup, sizeof(short int)*(p->xlen)*(p->ylen));
			for (i=0; i<11+q*(q-1); i++)
			{
				memcpy (p->map[i], map_backup[i], sizeof(int)*(p->xlen)*(p->ylen));
				memcpy (p->index[i], index_backup[i], sizeof(int)*(1+(p->xlen)*(p->ylen)));
				p->prob[i]=prob_backup[i];
			}
			p->coordinate=coordinate_backup;
			p->move=move_backup;
			p->steps=steps_backup;
			p->time=time_backup;
			p->energy=energy_backup;
			p->mag=mag_backup;
			for (i=0; i<q; i++)
			{
				p->num_value[i]=num_value_backup[i];
			}
			memcpy (p->T_vertices, T_vertices_backup, sizeof(short int)*(p->xlen)*(p->ylen));
			memcpy (p->T_vertices_2, T_vertices_2_backup, sizeof(short int)*(p->xlen)*(p->ylen));

			// Label the clusters.
			clusters (p, lattice, blinker_regions);

			// Get cluster statistics.
			cluster_stats (p, cluster_list, lattice, blinker_regions);

			// Initialize 'number_of_clusters'.
			number_of_clusters=0;
			
			// There must be at least one cluster.
			// 'number_of_clusters' gets incremented after each loop.
			blinker_regions_count=0;
			i=0;
			do
			{
				// Get the spins that are in the cluster.
				save_cluster_spins (p, cluster_spins, i, cluster_list, lattice);
				
				// Check if this is a blinker region.
				if ((cluster_list+number_of_clusters)->sign==1) blinker_regions_count++;

				// Increment 'number_of_clusters'.
				number_of_clusters++;
			}
			while (i<nxtimesny-1 && (cluster_list+(++i))->number_of_spins>0);

			// Save the number of blinker regions.
			*(number_of_blinker_regions+(number_of_blinker_regions_parameters->index)++)=(double)blinker_regions_count;
			number_of_blinker_regions_parameters->size+=sizeof(double);

			total_blinker_size=0;
			for (i=0; i<nxtimesny; i++)
			{
				if (*(blinker_regions+i)==1) total_blinker_size++;
			}

			// Save the volume of blinker regions.
			*(volume_of_blinker_regions+(volume_of_blinker_regions_parameters->index)++)=(double)total_blinker_size;
			volume_of_blinker_regions_parameters->size+=sizeof(double);

			return result_continue;
		}
		default: return result_continue;
	}
}

short int save_final_blinkers_2(short int command, struct save_final_blinkers2_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct save_final_blinkers2_data cmd_data;

	static short int data_index_parameters;
	static short int data_index_number_of_blinker_regions;
	static short int data_index_volume_of_blinker_regions;
	
	static double *number_of_blinker_regions;
	static struct save_final_blinkers2_parameters *number_of_blinker_regions_parameters;

	static double *volume_of_blinker_regions;
	static struct save_final_blinkers2_parameters *volume_of_blinker_regions_parameters;

	static double *blinker_size;
	static struct save_final_blinkers2_parameters *blinker_size_parameters;

	static double *blinker_perimeter;
	static struct save_final_blinkers2_parameters *blinker_perimeter_parameters;

	static double *blinker_value_1;
	static struct save_final_blinkers2_parameters *blinker_value_1_parameters;

	static double *blinker_value_2;
	static struct save_final_blinkers2_parameters *blinker_value_2_parameters;

	static double *blinker_size_index;
	static struct save_final_blinkers2_parameters *blinker_size_index_parameters;

	static double *spin_type_5;
	static struct save_final_blinkers2_parameters *spin_type_5_parameters;

	static struct run_parameters *p;
	static int *p_size;
	
	static int i, j, k, count, nxtimesny;

	static short int q;

	static int number_of_clusters;
	
	static int *lattice;
	static struct cluster_stats *cluster_list;
	static short int *cluster_spins;
	
	static unsigned long int rng_seed_backup;
	static unsigned long int rng_long_int_backup;
	static double rng_double_backup;

	static short int *spins_backup;
	static short int *state_backup;
	static int *map_backup[11+MAX_Q*(MAX_Q-1)];
	static int *index_backup[11+MAX_Q*(MAX_Q-1)];
	static double prob_backup[11+MAX_Q*(MAX_Q-1)];
	static int coordinate_backup;
	static short int move_backup;
	static unsigned long int steps_backup;
	static double time_backup;
	static double energy_backup;
	static int mag_backup;
	static int num_value_backup[MAX_Q];
	static short int *T_vertices_backup;
	static short int *T_vertices_2_backup;

	static short int *spins_plus_field;
	static short int *spins_minus_field;

	static short int *blinker_regions;

	static short int bias;

	static short int eligible_spins;

	static short int run_force;

	static int blinker_regions_count;
	static int total_blinker_size;

	// This array has length xlen*ylen.
	// It is used to determine the two possible values that the spins in each blinker region can take.
	static short int *blinker_region_values[2];

	switch (command)
	{
		case command_info: {printf("save_final_blinkers_2\n"); return result_continue;}
		case command_insert:
		{
			memcpy(&cmd_data, command_data, sizeof(struct save_final_blinkers2_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;

			nxtimesny=(p->xlen)*(p->ylen);

			q=p->q;

			lattice=(int*)malloc (sizeof(int)*nxtimesny);
			cluster_list=(struct cluster_stats*)malloc (sizeof(struct cluster_stats)*nxtimesny);
			cluster_spins=(short int*)malloc (sizeof(short int)*nxtimesny);
			
			// Allocate memory.
			spins_backup=(short int*)malloc (sizeof(short int)*(p->xlen)*(p->ylen));
			state_backup=(short int*)malloc (sizeof(short int)*(p->xlen)*(p->ylen));
			for (i=0; i<11+q*(q-1); i++)
			{
				map_backup[i]=(int*)malloc (sizeof(int)*(p->xlen)*(p->ylen));
				index_backup[i]=(int*)malloc (sizeof(int)*(1+(p->xlen)*(p->ylen)));
			}
			T_vertices_backup=(short int*)malloc (sizeof(short int)*(p->xlen)*(p->ylen));
			T_vertices_2_backup=(short int*)malloc (sizeof(short int)*(p->xlen)*(p->ylen));

			// These are used for storing the lattice after application of plus and minus bias fields.
			spins_plus_field=(short int*)malloc (sizeof(short int)*(p->xlen)*(p->ylen));
			spins_minus_field=(short int*)malloc (sizeof(short int)*(p->xlen)*(p->ylen));

			// This is used for saving the blinker regions.
			// A `0' value means that the lattice site is not part of a blinker region.
			// A `1' value means that the lattice site is part of a blinker region.
			blinker_regions=(short int*)malloc (sizeof(short int)*(p->xlen)*(p->ylen));

			// These two arrays are used to determine the two possible values
			// that the spins in each blinker region can take.
			blinker_region_values[0]=(short int*)malloc (sizeof(short int)*(p->xlen)*(p->ylen));
			blinker_region_values[1]=(short int*)malloc (sizeof(short int)*(p->xlen)*(p->ylen));

			// Initialize these arrays to MAX_Q.
			// Since MAX_Q is not a valid value for any spin, MAX_Q signals
			// that a spin value for the blinker region has not yet been recorded.
			for (i=0; i<(p->xlen)*(p->ylen); i++)
			{
				*(blinker_region_values[0]+i)=MAX_Q;
				*(blinker_region_values[1]+i)=MAX_Q;
			}

			return result_continue;
		}
		case command_begin:
		{
			if ((p->xlen)*(p->ylen)!=nxtimesny)
			{
				nxtimesny=(p->xlen)*(p->ylen);

				lattice=(int*)realloc (lattice, sizeof(int)*nxtimesny);
				cluster_list=(struct cluster_stats*)realloc (cluster_list, sizeof(struct cluster_stats)*nxtimesny);
				cluster_spins=(short int*)realloc (cluster_spins, sizeof(short int)*nxtimesny);
			}
			
			if (data_output(data, data_parameters, &number_of_blinker_regions, &number_of_blinker_regions_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_blinkers2_parameters), cmd_data.data_index_number_of_blinker_regions, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &volume_of_blinker_regions, &volume_of_blinker_regions_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_blinkers2_parameters), cmd_data.data_index_volume_of_blinker_regions, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &blinker_size, &blinker_size_parameters, cmd_data.max_blinkers*sizeof(double), sizeof(struct save_final_blinkers2_parameters), cmd_data.data_index_blinker_size, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &blinker_perimeter, &blinker_perimeter_parameters, cmd_data.max_blinkers*sizeof(double), sizeof(struct save_final_blinkers2_parameters), cmd_data.data_index_blinker_perimeter, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &blinker_value_1, &blinker_value_1_parameters, cmd_data.max_blinkers*sizeof(double), sizeof(struct save_final_blinkers2_parameters), cmd_data.data_index_blinker_value_1, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &blinker_value_2, &blinker_value_2_parameters, cmd_data.max_blinkers*sizeof(double), sizeof(struct save_final_blinkers2_parameters), cmd_data.data_index_blinker_value_2, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &blinker_size_index, &blinker_size_index_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_blinkers2_parameters), cmd_data.data_index_blinker_size_index, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &spin_type_5, &spin_type_5_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_blinkers2_parameters), cmd_data.data_index_spin_type_5, data_size)==1) return result_error;
			
			number_of_blinker_regions_parameters->index=0;
			number_of_blinker_regions_parameters->size=0;
			
			volume_of_blinker_regions_parameters->index=0;
			volume_of_blinker_regions_parameters->size=0;
			
			blinker_size_parameters->index=0;
			blinker_size_parameters->size=0;
			
			blinker_perimeter_parameters->index=0;
			blinker_perimeter_parameters->size=0;
			
			blinker_value_1_parameters->index=0;
			blinker_value_1_parameters->size=0;
			
			blinker_value_2_parameters->index=0;
			blinker_value_2_parameters->size=0;
			
			blinker_size_index_parameters->index=0;
			blinker_size_index_parameters->size=0;
			
			spin_type_5_parameters->index=0;
			spin_type_5_parameters->size=0;
			
			return result_continue;
		}
		case command_delete:
		{
			free (cluster_spins);
			free (cluster_list);
			free (lattice);
			return result_continue;
		}
		case command_run:
		{
			// Backup all simulation data.
			memcpy (spins_backup, p->spins, sizeof(short int)*(p->xlen)*(p->ylen));
			memcpy (state_backup, p->state, sizeof(short int)*(p->xlen)*(p->ylen));
			for (i=0; i<11+q*(q-1); i++)
			{
				memcpy (map_backup[i], p->map[i], sizeof(int)*(p->xlen)*(p->ylen));
				memcpy (index_backup[i], p->index[i], sizeof(int)*(1+(p->xlen)*(p->ylen)));
				prob_backup[i]=p->prob[i];
			}
			coordinate_backup=p->coordinate;
			move_backup=p->move;
			steps_backup=p->steps;
			time_backup=p->time;
			energy_backup=p->energy;
			mag_backup=p->mag;
			for (i=0; i<q; i++)
			{
				num_value_backup[i]=p->num_value[i];
			}
			memcpy (T_vertices_backup, p->T_vertices, sizeof(short int)*(p->xlen)*(p->ylen));
			memcpy (T_vertices_2_backup, p->T_vertices_2, sizeof(short int)*(p->xlen)*(p->ylen));

			// Initialize this to `0'.
			memset (blinker_regions, 0, sizeof(short int)*(p->xlen)*(p->ylen));

			for (bias=0; bias<2*q; bias++)
			{
				run_force=0;

				eligible_spins=0;

				for (i=0; i<q*(q-1); i++)
				{
					p->prob[11+i]=0;
				}

				if (bias%2==0)
				{
					for (i=0; i<q*(q-1); i++)
					{
						j=i/(q-1);
						k=((i%(q-1))+j+1)%q;

						if (j!=k && k==bias/2)
						{
							p->prob[11+i]=0.5;

							if (*(p->index[11+i])>0) eligible_spins++;
						}
					}

					if (eligible_spins>0) run_force=1;
				}
				else
				{
					for (i=0; i<q-1; i++)
					{
						p->prob[11+(bias/2)*(q-1)+i]=0.5;

						if (*(p->index[11+(bias/2)*(q-1)+i])>0) eligible_spins++;
					}

					if (eligible_spins>0) run_force=1;
				}

				// Check if there are eligible spins.
				if (run_force==1)
				{
					// Backup the state of the random number generator.
					rng_seed_backup=(p->rng_struct)->seed;
					rng_long_int_backup=(p->rng_struct)->current_long_int;
					rng_double_backup=(p->rng_struct)->current_double;
					(p->rng_struct)->rng_backup();
					
					// Drive the system with a bias field.
					if (run(	0,
							0,
							0,
							0,
							0,
							p,
							0)
						==result_error) return result_error;

					// Restore the state of the random number generator.
					(p->rng_struct)->seed=rng_seed_backup;
					(p->rng_struct)->current_long_int=rng_long_int_backup;
					(p->rng_struct)->current_double=rng_double_backup;
					(p->rng_struct)->rng_restore();
				}

				if (bias%2==0)
				{
					memcpy (spins_plus_field, p->spins, sizeof(short int)*(p->xlen)*(p->ylen));
				}
				else
				{
					memcpy (spins_minus_field, p->spins, sizeof(short int)*(p->xlen)*(p->ylen));

					for (i=0; i<nxtimesny; i++)
					{
						if (*(spins_plus_field+i)!=*(spins_minus_field+i))
						{
							*(blinker_regions+i)=1;

							if (*(blinker_region_values[0]+i)==MAX_Q) *(blinker_region_values[0]+i)=bias/2;
							// In case there are pseudo-blinkers, only save the
							// first two possible spin types.
							else if (*(blinker_region_values[1]+i)==MAX_Q) *(blinker_region_values[1]+i)=bias/2;
						}
					}
				}
			}
			
			// Restore the original lattice.
			memcpy (p->spins, spins_backup, sizeof(short int)*(p->xlen)*(p->ylen));
			memcpy (p->state, state_backup, sizeof(short int)*(p->xlen)*(p->ylen));
			for (i=0; i<11+q*(q-1); i++)
			{
				memcpy (p->map[i], map_backup[i], sizeof(int)*(p->xlen)*(p->ylen));
				memcpy (p->index[i], index_backup[i], sizeof(int)*(1+(p->xlen)*(p->ylen)));
				p->prob[i]=prob_backup[i];
			}
			p->coordinate=coordinate_backup;
			p->move=move_backup;
			p->steps=steps_backup;
			p->time=time_backup;
			p->energy=energy_backup;
			p->mag=mag_backup;
			for (i=0; i<q; i++)
			{
				p->num_value[i]=num_value_backup[i];
			}
			memcpy (p->T_vertices, T_vertices_backup, sizeof(short int)*(p->xlen)*(p->ylen));
			memcpy (p->T_vertices_2, T_vertices_2_backup, sizeof(short int)*(p->xlen)*(p->ylen));

			// Label the clusters.
			clusters (p, lattice, blinker_regions);

			// Get cluster statistics.
			cluster_stats (p, cluster_list, lattice, blinker_regions);

			// Initialize 'number_of_clusters'.
			number_of_clusters=0;
			
			// Save the index into the blinker size list.
			*(blinker_size_index+(blinker_size_index_parameters->index)++)=(double)(blinker_size_parameters->index);
			blinker_size_index_parameters->size+=sizeof(double);

			// There must be at least one cluster.
			// 'number_of_clusters' gets incremented after each loop.
			blinker_regions_count=0;
			i=0;
			do
			{
				// Get the spins that are in the cluster.
				save_cluster_spins (p, cluster_spins, i, cluster_list, lattice);
				
				// Check if this is a blinker region.
				if ((cluster_list+number_of_clusters)->sign==1)
				{
					blinker_regions_count++;

					// Save the size of the blinker.
					*(blinker_size+(blinker_size_parameters->index)++)=(double)((cluster_list+i)->number_of_spins);
					blinker_size_parameters->size+=sizeof(double);

					// Save the perimeter of the blinker.
					*(blinker_perimeter+(blinker_perimeter_parameters->index)++)=(double)((cluster_list+i)->perimeter);
					blinker_perimeter_parameters->size+=sizeof(double);

					// Save the sign 1 of the blinker.
					*(blinker_value_1+(blinker_value_1_parameters->index)++)=(double)(*(blinker_region_values[0]+(cluster_list+i)->label));
					blinker_value_1_parameters->size+=sizeof(double);

					// Save the sign 2 of the blinker.
					*(blinker_value_2+(blinker_value_2_parameters->index)++)=(double)(*(blinker_region_values[1]+(cluster_list+i)->label));
					blinker_value_2_parameters->size+=sizeof(double);
				}

				// Increment 'number_of_clusters'.
				number_of_clusters++;
			}
			while (i<nxtimesny-1 && (cluster_list+(++i))->number_of_spins>0);

			// Save the number of blinker regions.
			*(number_of_blinker_regions+(number_of_blinker_regions_parameters->index)++)=(double)blinker_regions_count;
			number_of_blinker_regions_parameters->size+=sizeof(double);

			total_blinker_size=0;
			for (i=0; i<nxtimesny; i++)
			{
				if (*(blinker_regions+i)==1) total_blinker_size++;
			}

			// Save the volume of blinker regions.
			*(volume_of_blinker_regions+(volume_of_blinker_regions_parameters->index)++)=(double)total_blinker_size;
			volume_of_blinker_regions_parameters->size+=sizeof(double);

			// Save the number of type 5 spins.
			*(spin_type_5+(spin_type_5_parameters->index)++)=(double)(*(p->index[5]));
			spin_type_5_parameters->size+=sizeof(double);

			return result_continue;
		}
		default: return result_continue;
	}
}

short int save_final_clusters(short int command, struct save_final_clusters_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct save_final_clusters_data cmd_data;

	static short int data_index_parameters;
	static short int data_index_number_of_clusters;
	
	static struct run_parameters *parameters;
	static int *parameters_size;
	
	static int i, count, nxtimesny;

	static int number_of_clusters;
	
	static int *lattice;
	static struct cluster_stats *cluster_list;
	static short int *cluster_spins;
	
	static double *cluster_count;
	static struct save_final_clusters_parameters *cluster_count_parameters;
	
	static double *cluster_size;
	static struct save_final_clusters_parameters *cluster_size_parameters;
	
	static double *cluster_sign;
	static struct save_final_clusters_parameters *cluster_sign_parameters;
	
	static double *cluster_size_index;
	static struct save_final_clusters_parameters *cluster_size_index_parameters;
	
	switch (command)
	{
		case command_info: {printf("save_final_clusters\n"); return result_continue;}
		case command_insert:
		{
			memcpy(&cmd_data, command_data, sizeof(struct save_final_clusters_data));

			if (data_input(data, data_parameters, &parameters, &parameters_size, cmd_data.data_index_parameters, data_size)==1) return result_error;

			nxtimesny=(parameters->xlen)*(parameters->ylen);

			lattice=(int*)malloc (sizeof(int)*nxtimesny);
			cluster_list=(struct cluster_stats*)malloc (sizeof(struct cluster_stats)*nxtimesny);
			cluster_spins=(short int*)malloc (sizeof(short int)*nxtimesny);
			
			return result_continue;
		}
		case command_begin:
		{
			if ((parameters->xlen)*(parameters->ylen)!=nxtimesny)
			{
				nxtimesny=(parameters->xlen)*(parameters->ylen);

				lattice=(int*)realloc (lattice, sizeof(int)*nxtimesny);
				cluster_list=(struct cluster_stats*)realloc (cluster_list, sizeof(struct cluster_stats)*nxtimesny);
				cluster_spins=(short int*)realloc (cluster_spins, sizeof(short int)*nxtimesny);
			}
			
			if (data_output(data, data_parameters, &cluster_count, &cluster_count_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_clusters_parameters), cmd_data.data_index_number_of_clusters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &cluster_size, &cluster_size_parameters, cmd_data.max_clusters*sizeof(double), sizeof(struct save_final_clusters_parameters), cmd_data.data_index_cluster_size, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &cluster_sign, &cluster_sign_parameters, cmd_data.max_clusters*sizeof(double), sizeof(struct save_final_clusters_parameters), cmd_data.data_index_cluster_sign, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &cluster_size_index, &cluster_size_index_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_clusters_parameters), cmd_data.data_index_cluster_size_index, data_size)==1) return result_error;
			
			cluster_count_parameters->index=0;
			cluster_count_parameters->size=0;
			
			cluster_size_parameters->index=0;
			cluster_size_parameters->size=0;
			
			cluster_sign_parameters->index=0;
			cluster_sign_parameters->size=0;
			
			cluster_size_index_parameters->index=0;
			cluster_size_index_parameters->size=0;
			
			return result_continue;
		}
		case command_delete:
		{
			free (cluster_spins);
			free (cluster_list);
			free (lattice);
			return result_continue;
		}
		case command_run:
		{
			// Label the clusters.
			clusters (parameters, lattice, parameters->spins);

			// Get cluster statistics.
			cluster_stats (parameters, cluster_list, lattice, parameters->spins);

			// Initialize 'number_of_clusters'.
			number_of_clusters=0;
			
			// Save the index into the cluster size list.
			*(cluster_size_index+(cluster_size_index_parameters->index)++)=(double)(cluster_size_parameters->index);
			cluster_size_index_parameters->size+=sizeof(double);

			// There must be at least one cluster.
			// 'number_of_clusters' gets incremented after each loop.
			i=0;
			do
			{
				// Get the spins that are in the cluster.
				save_cluster_spins (parameters, cluster_spins, i, cluster_list, lattice);
				
				// Increment 'number_of_clusters'.
				number_of_clusters++;

				// Save the size of the cluster.
				*(cluster_size+(cluster_size_parameters->index)++)=(double)((cluster_list+i)->number_of_spins);
				cluster_size_parameters->size+=sizeof(double);

				// Save the sign of the cluster.
				*(cluster_sign+(cluster_sign_parameters->index)++)=(double)((cluster_list+i)->sign);
				cluster_sign_parameters->size+=sizeof(double);
			}
			while (i<nxtimesny-1 && (cluster_list+(++i))->number_of_spins>0);

			// Save the number of clusters.
			*(cluster_count+(cluster_count_parameters->index)++)=(double)number_of_clusters;
			cluster_count_parameters->size+=sizeof(double);

			return result_continue;
		}
		default: return result_continue;
	}
}

short int save_final_clusters_percolation(short int command, struct save_final_clusters_percolation_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct save_final_clusters_percolation_data cmd_data;

	static short int data_index_parameters;
	static short int data_index_number_of_clusters;
	
	static struct run_parameters *parameters;
	static int *parameters_size;
	
	static int i, count, nxtimesny;

	static int number_of_clusters;
	
	static int *lattice;
	static struct cluster_stats *cluster_list;
	static short int *cluster_spins;

	static int rank;
	
	static int *displacement_x;
	static int *displacement_y;
	static int *winding_a_lattice;
	static int *winding_b_lattice;
	static int stripes_count;
	static int winding_a_max;
	static int winding_b_max;

	static double *cluster_count;
	static struct save_final_clusters_parameters *cluster_count_parameters;
	
	static double *cluster_size;
	static struct save_final_clusters_parameters *cluster_size_parameters;
	
	static double *cluster_perimeter;
	static struct save_final_clusters_parameters *cluster_perimeter_parameters;
	
	static double *cluster_value;
	static struct save_final_clusters_parameters *cluster_value_parameters;
	
	static double *cluster_winding_a;
	static struct save_final_clusters_parameters *cluster_winding_a_parameters;
	
	static double *cluster_winding_b;
	static struct save_final_clusters_parameters *cluster_winding_b_parameters;
	
	static double *cluster_size_index;
	static struct save_final_clusters_parameters *cluster_size_index_parameters;
	
	static double *number_of_stripes;
	static struct save_final_clusters_parameters *number_of_stripes_parameters;
	
	static double *winding_a;
	static struct save_final_clusters_parameters *winding_a_parameters;
	
	static double *winding_b;
	static struct save_final_clusters_parameters *winding_b_parameters;
	
	switch (command)
	{
		case command_info: {printf("save_final_clusters_percolation\n"); return result_continue;}
		case command_insert:
		{
			memcpy(&cmd_data, command_data, sizeof(struct save_final_clusters_percolation_data));

			if (data_input(data, data_parameters, &parameters, &parameters_size, cmd_data.data_index_parameters, data_size)==1) return result_error;

			nxtimesny=(parameters->xlen)*(parameters->ylen);

			lattice=(int*)malloc (sizeof(int)*nxtimesny);
			cluster_list=(struct cluster_stats*)malloc (sizeof(struct cluster_stats)*nxtimesny);
			cluster_spins=(short int*)malloc (sizeof(short int)*nxtimesny);
			
			displacement_x=(int*)malloc (sizeof(int)*nxtimesny);
			displacement_y=(int*)malloc (sizeof(int)*nxtimesny);
			winding_a_lattice=(int*)malloc (sizeof(int)*nxtimesny);
			winding_b_lattice=(int*)malloc (sizeof(int)*nxtimesny);

			// Get this process's rank.
//			MPI_Comm_rank(MPI_COMM_WORLD, &rank);

			rank=1;
			
			return result_continue;
		}
		case command_begin:
		{
			if ((parameters->xlen)*(parameters->ylen)!=nxtimesny)
			{
				nxtimesny=(parameters->xlen)*(parameters->ylen);

				lattice=(int*)realloc (lattice, sizeof(int)*nxtimesny);
				cluster_list=(struct cluster_stats*)realloc (cluster_list, sizeof(struct cluster_stats)*nxtimesny);
				cluster_spins=(short int*)realloc (cluster_spins, sizeof(short int)*nxtimesny);
			}
			
			if (data_output(data, data_parameters, &cluster_count, &cluster_count_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_clusters_parameters), cmd_data.data_index_number_of_clusters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &cluster_size, &cluster_size_parameters, cmd_data.max_clusters*sizeof(double), sizeof(struct save_final_clusters_parameters), cmd_data.data_index_cluster_size, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &cluster_perimeter, &cluster_perimeter_parameters, cmd_data.max_clusters*sizeof(double), sizeof(struct save_final_clusters_parameters), cmd_data.data_index_cluster_perimeter, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &cluster_value, &cluster_value_parameters, cmd_data.max_clusters*sizeof(double), sizeof(struct save_final_clusters_parameters), cmd_data.data_index_cluster_value, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &cluster_winding_a, &cluster_winding_a_parameters, cmd_data.max_clusters*sizeof(double), sizeof(struct save_final_clusters_parameters), cmd_data.data_index_cluster_winding_a, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &cluster_winding_b, &cluster_winding_b_parameters, cmd_data.max_clusters*sizeof(double), sizeof(struct save_final_clusters_parameters), cmd_data.data_index_cluster_winding_b, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &cluster_size_index, &cluster_size_index_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_clusters_parameters), cmd_data.data_index_cluster_size_index, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &number_of_stripes, &number_of_stripes_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_clusters_parameters), cmd_data.data_index_number_of_stripes, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &winding_a, &winding_a_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_clusters_parameters), cmd_data.data_index_winding_a, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &winding_b, &winding_b_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_clusters_parameters), cmd_data.data_index_winding_b, data_size)==1) return result_error;
			
			cluster_count_parameters->index=0;
			cluster_count_parameters->size=0;
			
			cluster_size_parameters->index=0;
			cluster_size_parameters->size=0;
			
			cluster_perimeter_parameters->index=0;
			cluster_perimeter_parameters->size=0;
			
			cluster_value_parameters->index=0;
			cluster_value_parameters->size=0;
			
			cluster_winding_a_parameters->index=0;
			cluster_winding_a_parameters->size=0;
			
			cluster_winding_b_parameters->index=0;
			cluster_winding_b_parameters->size=0;
			
			cluster_size_index_parameters->index=0;
			cluster_size_index_parameters->size=0;
			
			number_of_stripes_parameters->index=0;
			number_of_stripes_parameters->size=0;
			
			winding_a_parameters->index=0;
			winding_a_parameters->size=0;
			
			winding_b_parameters->index=0;
			winding_b_parameters->size=0;

			// Initialize the cluster value array so that each value is MAX_Q.
			// If any other routine checks a cluster value and finds MAX_Q,
			// then it knows that this does not correspond to an actual cluster.
			for (i=0; i<cmd_data.max_clusters; i++)
			{
				*(cluster_value+i)=MAX_Q;
			}
			
			return result_continue;
		}
		case command_delete:
		{
			free (cluster_spins);
			free (cluster_list);
			free (lattice);
			return result_continue;
		}
		case command_run:
		{
			// Label the clusters.
			clusters_percolation (parameters, lattice, parameters->spins, displacement_x, displacement_y, winding_a_lattice, winding_b_lattice, &stripes_count, &winding_a_max, &winding_b_max);

			// Get cluster statistics.
			cluster_stats_percolation (parameters, cluster_list, lattice, parameters->spins, winding_a_lattice, winding_b_lattice);

			// Initialize 'number_of_clusters'.
			number_of_clusters=0;
			
			// Save the index into the cluster size list.
			*(cluster_size_index+(cluster_size_index_parameters->index)++)=(double)((rank-1)*(cmd_data.max_clusters)+(cluster_size_parameters->index));
			cluster_size_index_parameters->size+=sizeof(double);

			// There must be at least one cluster.
			// 'number_of_clusters' gets incremented after each loop.
			i=0;
			do
			{
				// Get the spins that are in the cluster.
				save_cluster_spins (parameters, cluster_spins, i, cluster_list, lattice);
				
				// Increment 'number_of_clusters'.
				number_of_clusters++;

				// Save the size of the cluster.
				*(cluster_size+(cluster_size_parameters->index)++)=(double)((cluster_list+i)->number_of_spins);
				cluster_size_parameters->size+=sizeof(double);

				// Save the perimeter of the cluster.
				*(cluster_perimeter+(cluster_perimeter_parameters->index)++)=(double)((cluster_list+i)->perimeter);
				cluster_perimeter_parameters->size+=sizeof(double);

				// Save the value of the cluster.
				*(cluster_value+(cluster_value_parameters->index)++)=(double)((cluster_list+i)->sign);
				cluster_value_parameters->size+=sizeof(double);

				// Save the winding number a of the cluster.
				*(cluster_winding_a+(cluster_winding_a_parameters->index)++)=(double)((cluster_list+i)->winding_a);
				cluster_winding_a_parameters->size+=sizeof(double);

				// Save the winding number b of the cluster.
				*(cluster_winding_b+(cluster_winding_b_parameters->index)++)=(double)((cluster_list+i)->winding_b);
				cluster_winding_b_parameters->size+=sizeof(double);
			}
			while (i<nxtimesny-1 && (cluster_list+(++i))->number_of_spins>0);

			// Save the number of clusters.
			*(cluster_count+(cluster_count_parameters->index)++)=(double)number_of_clusters;
			cluster_count_parameters->size+=sizeof(double);

			// Save the number of stripes.
			*(number_of_stripes+(number_of_stripes_parameters->index)++)=(double)stripes_count;
			number_of_stripes_parameters->size+=sizeof(double);

			// Save the horizontal winding number.
			*(winding_a+(winding_a_parameters->index)++)=(double)winding_a_max;
			winding_a_parameters->size+=sizeof(double);

			// Save the vertical winding number.
			*(winding_b+(winding_b_parameters->index)++)=(double)winding_b_max;
			winding_b_parameters->size+=sizeof(double);

			return result_continue;
		}
		default: return result_continue;
	}
}

short int save_final_energy(short int command, struct save_final_energy_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct save_final_energy_data cmd_data;
	
	static double *final_energy;

	static struct run_parameters *parameters;
	static int *parameters_size;
	
	static struct save_final_energy_parameters *final_energy_parameters;

	switch (command)
	{
		case command_info: {printf("save_final_energy\n"); return result_continue;}
		case command_insert:
		{
			memcpy(&cmd_data, command_data, sizeof(struct save_final_energy_data));
			
			return result_continue;
		}
		case command_begin:
		{
			if (data_input(data, data_parameters, &parameters, &parameters_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &final_energy, &final_energy_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_energy_parameters), cmd_data.data_index_final_energy, data_size)==1) return result_error;
			
			final_energy_parameters->index=0;
			final_energy_parameters->size=0;
			
			return result_continue;
		}
		case command_run:
		{
			*(final_energy+(final_energy_parameters->index))=parameters->energy;
			
			(final_energy_parameters->index)++;
			final_energy_parameters->size+=sizeof(double);

			return result_continue;
		}
		default: return result_continue;
	}
}

short int save_final_mag(short int command, struct save_final_mag_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct save_final_mag_data cmd_data;
	
	static double *final_mag;

	static struct run_parameters *parameters;
	static int *parameters_size;
	
	static struct save_final_mag_parameters *final_mag_parameters;

	switch (command)
	{
		case command_info: {printf("save_final_mag\n"); return result_continue;}
		case command_insert:
		{
			memcpy(&cmd_data, command_data, sizeof(struct save_final_mag_data));
			
			return result_continue;
		}
		case command_begin:
		{
			if (data_input(data, data_parameters, &parameters, &parameters_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &final_mag, &final_mag_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_mag_parameters), cmd_data.data_index_final_mag, data_size)==1) return result_error;
			
			final_mag_parameters->index=0;
			final_mag_parameters->size=0;
			
			return result_continue;
		}
		case command_run:
		{
			*(final_mag+(final_mag_parameters->index))=(double)parameters->mag;
			
			(final_mag_parameters->index)++;
			final_mag_parameters->size+=sizeof(double);

			return result_continue;
		}
		default: return result_continue;
	}
}

short int save_final_mag_abs(short int command, struct save_final_mag_abs_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct save_final_mag_abs_data cmd_data;
	
	static double *final_mag_abs;

	static struct run_parameters *parameters;
	static int *parameters_size;
	
	static struct save_final_mag_abs_parameters *final_mag_abs_parameters;

	switch (command)
	{
		case command_info: {printf("save_final_mag_abs\n"); return result_continue;}
		case command_insert:
		{
			memcpy(&cmd_data, command_data, sizeof(struct save_final_mag_abs_data));
			
			return result_continue;
		}
		case command_begin:
		{
			if (data_input(data, data_parameters, &parameters, &parameters_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &final_mag_abs, &final_mag_abs_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_mag_abs_parameters), cmd_data.data_index_final_mag, data_size)==1) return result_error;
			
			final_mag_abs_parameters->index=0;
			final_mag_abs_parameters->size=0;
			
			return result_continue;
		}
		case command_run:
		{
			*(final_mag_abs+(final_mag_abs_parameters->index))=(double)abs(parameters->mag);
			
			(final_mag_abs_parameters->index)++;
			final_mag_abs_parameters->size+=sizeof(double);

			return result_continue;
		}
		default: return result_continue;
	}
}

short int save_final_number_0(short int command, struct save_final_number_spin_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct save_final_number_spin_data cmd_data;
	
	static double *final_number_spin;

	static struct run_parameters *parameters;
	static int *parameters_size;
	
	static struct save_final_number_spin_parameters *final_number_spin_parameters;

	switch (command)
	{
		case command_info: {printf("save_final_number_0\n"); return result_continue;}
		case command_insert:
		{
			memcpy(&cmd_data, command_data, sizeof(struct save_final_number_spin_data));
			
			return result_continue;
		}
		case command_begin:
		{
			if (data_input(data, data_parameters, &parameters, &parameters_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &final_number_spin, &final_number_spin_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_number_spin_parameters), cmd_data.data_index_final_number_spin, data_size)==1) return result_error;
			
			final_number_spin_parameters->index=0;
			final_number_spin_parameters->size=0;
			
			return result_continue;
		}
		case command_run:
		{
			*(final_number_spin+(final_number_spin_parameters->index))=(double)parameters->num_value[0];
			
			(final_number_spin_parameters->index)++;
			final_number_spin_parameters->size+=sizeof(double);

			return result_continue;
		}
		default: return result_continue;
	}
}

short int save_final_number_1(short int command, struct save_final_number_spin_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct save_final_number_spin_data cmd_data;
	
	static double *final_number_spin;

	static struct run_parameters *parameters;
	static int *parameters_size;
	
	static struct save_final_number_spin_parameters *final_number_spin_parameters;

	switch (command)
	{
		case command_info: {printf("save_final_number_1\n"); return result_continue;}
		case command_insert:
		{
			memcpy(&cmd_data, command_data, sizeof(struct save_final_number_spin_data));
			
			return result_continue;
		}
		case command_begin:
		{
			if (data_input(data, data_parameters, &parameters, &parameters_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &final_number_spin, &final_number_spin_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_number_spin_parameters), cmd_data.data_index_final_number_spin, data_size)==1) return result_error;
			
			final_number_spin_parameters->index=0;
			final_number_spin_parameters->size=0;
			
			return result_continue;
		}
		case command_run:
		{
			*(final_number_spin+(final_number_spin_parameters->index))=(double)parameters->num_value[1];
			
			(final_number_spin_parameters->index)++;
			final_number_spin_parameters->size+=sizeof(double);

			return result_continue;
		}
		default: return result_continue;
	}
}

short int save_final_number_2(short int command, struct save_final_number_spin_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct save_final_number_spin_data cmd_data;
	
	static double *final_number_spin;

	static struct run_parameters *parameters;
	static int *parameters_size;
	
	static struct save_final_number_spin_parameters *final_number_spin_parameters;

	switch (command)
	{
		case command_info: {printf("save_final_number_2\n"); return result_continue;}
		case command_insert:
		{
			memcpy(&cmd_data, command_data, sizeof(struct save_final_number_spin_data));
			
			return result_continue;
		}
		case command_begin:
		{
			if (data_input(data, data_parameters, &parameters, &parameters_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &final_number_spin, &final_number_spin_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_number_spin_parameters), cmd_data.data_index_final_number_spin, data_size)==1) return result_error;
			
			final_number_spin_parameters->index=0;
			final_number_spin_parameters->size=0;
			
			return result_continue;
		}
		case command_run:
		{
			*(final_number_spin+(final_number_spin_parameters->index))=(double)parameters->num_value[2];
			
			(final_number_spin_parameters->index)++;
			final_number_spin_parameters->size+=sizeof(double);

			return result_continue;
		}
		default: return result_continue;
	}
}

short int save_final_number_3(short int command, struct save_final_number_spin_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct save_final_number_spin_data cmd_data;
	
	static double *final_number_spin;

	static struct run_parameters *parameters;
	static int *parameters_size;
	
	static struct save_final_number_spin_parameters *final_number_spin_parameters;

	switch (command)
	{
		case command_info: {printf("save_final_number_3\n"); return result_continue;}
		case command_insert:
		{
			memcpy(&cmd_data, command_data, sizeof(struct save_final_number_spin_data));
			
			return result_continue;
		}
		case command_begin:
		{
			if (data_input(data, data_parameters, &parameters, &parameters_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &final_number_spin, &final_number_spin_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_number_spin_parameters), cmd_data.data_index_final_number_spin, data_size)==1) return result_error;
			
			final_number_spin_parameters->index=0;
			final_number_spin_parameters->size=0;
			
			return result_continue;
		}
		case command_run:
		{
			*(final_number_spin+(final_number_spin_parameters->index))=(double)parameters->num_value[3];
			
			(final_number_spin_parameters->index)++;
			final_number_spin_parameters->size+=sizeof(double);

			return result_continue;
		}
		default: return result_continue;
	}
}

short int save_final_number_4(short int command, struct save_final_number_spin_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct save_final_number_spin_data cmd_data;
	
	static double *final_number_spin;

	static struct run_parameters *parameters;
	static int *parameters_size;
	
	static struct save_final_number_spin_parameters *final_number_spin_parameters;

	switch (command)
	{
		case command_info: {printf("save_final_number_4\n"); return result_continue;}
		case command_insert:
		{
			memcpy(&cmd_data, command_data, sizeof(struct save_final_number_spin_data));
			
			return result_continue;
		}
		case command_begin:
		{
			if (data_input(data, data_parameters, &parameters, &parameters_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &final_number_spin, &final_number_spin_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_number_spin_parameters), cmd_data.data_index_final_number_spin, data_size)==1) return result_error;
			
			final_number_spin_parameters->index=0;
			final_number_spin_parameters->size=0;
			
			return result_continue;
		}
		case command_run:
		{
			*(final_number_spin+(final_number_spin_parameters->index))=(double)parameters->num_value[4];
			
			(final_number_spin_parameters->index)++;
			final_number_spin_parameters->size+=sizeof(double);

			return result_continue;
		}
		default: return result_continue;
	}
}

short int save_final_number_5(short int command, struct save_final_number_spin_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct save_final_number_spin_data cmd_data;
	
	static double *final_number_spin;

	static struct run_parameters *parameters;
	static int *parameters_size;
	
	static struct save_final_number_spin_parameters *final_number_spin_parameters;

	switch (command)
	{
		case command_info: {printf("save_final_number_5\n"); return result_continue;}
		case command_insert:
		{
			memcpy(&cmd_data, command_data, sizeof(struct save_final_number_spin_data));
			
			return result_continue;
		}
		case command_begin:
		{
			if (data_input(data, data_parameters, &parameters, &parameters_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &final_number_spin, &final_number_spin_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_number_spin_parameters), cmd_data.data_index_final_number_spin, data_size)==1) return result_error;
			
			final_number_spin_parameters->index=0;
			final_number_spin_parameters->size=0;
			
			return result_continue;
		}
		case command_run:
		{
			*(final_number_spin+(final_number_spin_parameters->index))=(double)parameters->num_value[5];
			
			(final_number_spin_parameters->index)++;
			final_number_spin_parameters->size+=sizeof(double);

			return result_continue;
		}
		default: return result_continue;
	}
}

short int save_final_number_6(short int command, struct save_final_number_spin_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct save_final_number_spin_data cmd_data;
	
	static double *final_number_spin;

	static struct run_parameters *parameters;
	static int *parameters_size;
	
	static struct save_final_number_spin_parameters *final_number_spin_parameters;

	switch (command)
	{
		case command_info: {printf("save_final_number_6\n"); return result_continue;}
		case command_insert:
		{
			memcpy(&cmd_data, command_data, sizeof(struct save_final_number_spin_data));
			
			return result_continue;
		}
		case command_begin:
		{
			if (data_input(data, data_parameters, &parameters, &parameters_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &final_number_spin, &final_number_spin_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_number_spin_parameters), cmd_data.data_index_final_number_spin, data_size)==1) return result_error;
			
			final_number_spin_parameters->index=0;
			final_number_spin_parameters->size=0;
			
			return result_continue;
		}
		case command_run:
		{
			*(final_number_spin+(final_number_spin_parameters->index))=(double)parameters->num_value[6];
			
			(final_number_spin_parameters->index)++;
			final_number_spin_parameters->size+=sizeof(double);

			return result_continue;
		}
		default: return result_continue;
	}
}

short int save_final_number_7(short int command, struct save_final_number_spin_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct save_final_number_spin_data cmd_data;
	
	static double *final_number_spin;

	static struct run_parameters *parameters;
	static int *parameters_size;
	
	static struct save_final_number_spin_parameters *final_number_spin_parameters;

	switch (command)
	{
		case command_info: {printf("save_final_number_7\n"); return result_continue;}
		case command_insert:
		{
			memcpy(&cmd_data, command_data, sizeof(struct save_final_number_spin_data));
			
			return result_continue;
		}
		case command_begin:
		{
			if (data_input(data, data_parameters, &parameters, &parameters_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &final_number_spin, &final_number_spin_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_number_spin_parameters), cmd_data.data_index_final_number_spin, data_size)==1) return result_error;
			
			final_number_spin_parameters->index=0;
			final_number_spin_parameters->size=0;
			
			return result_continue;
		}
		case command_run:
		{
			*(final_number_spin+(final_number_spin_parameters->index))=(double)parameters->num_value[7];
			
			(final_number_spin_parameters->index)++;
			final_number_spin_parameters->size+=sizeof(double);

			return result_continue;
		}
		default: return result_continue;
	}
}

short int save_final_number_8(short int command, struct save_final_number_spin_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct save_final_number_spin_data cmd_data;
	
	static double *final_number_spin;

	static struct run_parameters *parameters;
	static int *parameters_size;
	
	static struct save_final_number_spin_parameters *final_number_spin_parameters;

	switch (command)
	{
		case command_info: {printf("save_final_number_8\n"); return result_continue;}
		case command_insert:
		{
			memcpy(&cmd_data, command_data, sizeof(struct save_final_number_spin_data));
			
			return result_continue;
		}
		case command_begin:
		{
			if (data_input(data, data_parameters, &parameters, &parameters_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &final_number_spin, &final_number_spin_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_number_spin_parameters), cmd_data.data_index_final_number_spin, data_size)==1) return result_error;
			
			final_number_spin_parameters->index=0;
			final_number_spin_parameters->size=0;
			
			return result_continue;
		}
		case command_run:
		{
			*(final_number_spin+(final_number_spin_parameters->index))=(double)parameters->num_value[8];
			
			(final_number_spin_parameters->index)++;
			final_number_spin_parameters->size+=sizeof(double);

			return result_continue;
		}
		default: return result_continue;
	}
}

short int save_final_steps(short int command, struct save_final_steps_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct save_final_steps_data cmd_data;
	
	static double *final_steps;

	static struct run_parameters *parameters;
	static int *parameters_size;
	
	static struct save_final_steps_parameters *final_steps_parameters;

	static int i, mag;
	
	switch (command)
	{
		case command_info: {printf("save_final_steps\n"); return result_continue;}
		case command_insert:
		{
			memcpy(&cmd_data, command_data, sizeof(struct save_final_steps_data));
			
			return result_continue;
		}
		case command_begin:
		{
			if (data_input(data, data_parameters, &parameters, &parameters_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &final_steps, &final_steps_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_steps_parameters), cmd_data.data_index_final_steps, data_size)==1) return result_error;
			
			final_steps_parameters->index=0;
			final_steps_parameters->size=0;
			
			return result_continue;
		}
		case command_run:
		{
			*(final_steps+(final_steps_parameters->index))=(double)(parameters->steps);
			
			(final_steps_parameters->index)++;
			final_steps_parameters->size+=sizeof(double);

			return result_continue;
		}
		default: return result_continue;
	}
}

short int save_final_time(short int command, struct save_final_time_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct save_final_time_data cmd_data;
	
	static double *final_time;

	static struct run_parameters *parameters;
	static int *parameters_size;
	
	static struct save_final_time_parameters *final_time_parameters;

	static int i, mag;
	
	switch (command)
	{
		case command_info: {printf("save_final_time\n"); return result_continue;}
		case command_insert:
		{
			memcpy(&cmd_data, command_data, sizeof(struct save_final_time_data));
			
			return result_continue;
		}
		case command_begin:
		{
			if (data_input(data, data_parameters, &parameters, &parameters_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &final_time, &final_time_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_time_parameters), cmd_data.data_index_final_time, data_size)==1) return result_error;
			
			final_time_parameters->index=0;
			final_time_parameters->size=0;
			
			return result_continue;
		}
		case command_run:
		{
			*(final_time+(final_time_parameters->index))=parameters->time;
			
			(final_time_parameters->index)++;
			final_time_parameters->size+=sizeof(double);

			return result_continue;
		}
		default: return result_continue;
	}
}

short int save_final_T_vertices(short int command, struct save_final_T_vertices_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct save_final_T_vertices_data cmd_data;
	
	static double *final_T_vertices;

	static struct run_parameters *p;
	static int *p_size;
	
	static struct save_final_T_vertices_parameters *final_T_vertices_parameters;

	static int i, mag;
	
	static int count, num_diff;

	static int x, y, xlen, ylen;
	static int xminus1;
	static int yminus1;

	switch (command)
	{
		case command_info: {printf("save_final_T_vertices\n"); return result_continue;}
		case command_insert:
		{
			memcpy(&cmd_data, command_data, sizeof(struct save_final_T_vertices_data));
			
			return result_continue;
		}
		case command_begin:
		{
			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &final_T_vertices, &final_T_vertices_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_T_vertices_parameters), cmd_data.data_index_final_T_vertices, data_size)==1) return result_error;
			
			final_T_vertices_parameters->index=0;
			final_T_vertices_parameters->size=0;
			
			return result_continue;
		}
		case command_run:
		{
			// Count the number of T vertices in the system.
			count=0;
			for (y=0; y<ylen; y++)
			{
				for (x=0; x<xlen; x++)
				{
					// Determine the locations of the neighboring spins.
					if (x==0) xminus1=xlen-1; else xminus1=x-1;
					if (y==0) yminus1=ylen-1; else yminus1=y-1;

					num_diff=0;

					if (*(p->spins+x+y*xlen)!=*(p->spins+xminus1+y*xlen)) num_diff++;
					if (*(p->spins+xminus1+y*xlen)!=*(p->spins+xminus1+yminus1*xlen)) num_diff++;
					if (*(p->spins+xminus1+yminus1*xlen)!=*(p->spins+x+yminus1*xlen)) num_diff++;
					if (*(p->spins+x+yminus1*xlen)!=*(p->spins+x+y*xlen)) num_diff++;

					if (num_diff==3) {*(p->T_vertices+x+y*xlen)=1; count++;} else *(p->T_vertices+x+y*xlen)=0;
				}
			}

			*(final_T_vertices+(final_T_vertices_parameters->index))=(double)count;
			
			(final_T_vertices_parameters->index)++;
			final_T_vertices_parameters->size+=sizeof(double);

			return result_continue;
		}
		default: return result_continue;
	}
}

short int save_final_T_vertices_2(short int command, struct save_final_T_vertices_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct save_final_T_vertices_data cmd_data;
	
	static double *final_T_vertices;

	static struct run_parameters *p;
	static int *p_size;
	
	static struct save_final_T_vertices_parameters *final_T_vertices_parameters;

	static int i, mag;
	
	static int count, num_diff;

	static int x, y, xlen, ylen;
	static int xminus1, xminus2, xplus1;
	static int yminus1, yminus2, yplus1;

	switch (command)
	{
		case command_info: {printf("save_final_T_vertices_2\n"); return result_continue;}
		case command_insert:
		{
			memcpy(&cmd_data, command_data, sizeof(struct save_final_T_vertices_data));
			
			return result_continue;
		}
		case command_begin:
		{
			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &final_T_vertices, &final_T_vertices_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_T_vertices_parameters), cmd_data.data_index_final_T_vertices, data_size)==1) return result_error;
			
			final_T_vertices_parameters->index=0;
			final_T_vertices_parameters->size=0;
			
			return result_continue;
		}
		case command_run:
		{
			// Count the number of T vertices in the system.
			count=0;
			for (y=0; y<ylen; y++)
			{
				for (x=0; x<xlen; x++)
				{
					// Determine the locations of the neighboring spins.
					if (x==0) xminus1=xlen-1; else xminus1=x-1;
					if (x==0) xminus2=xlen-2; else xminus2=x-2;
					if (x==1) xminus2=xlen-1; else xminus2=x-2;
					if (x==p->xlen-1) xplus1=0; else xplus1=x+1;
					if (y==0) yminus1=ylen-1; else yminus1=y-1;
					if (y==0) yminus2=ylen-2; else yminus2=y-2;
					if (y==1) yminus2=ylen-1; else yminus2=y-2;
					if (y==p->ylen-1) yplus1=0; else yplus1=y+1;

					if (	*(p->spins+x+y*xlen)==*(p->spins+xplus1+y*xlen) && *(p->spins+x+y*xlen)==*(p->spins+x+yplus1*xlen) &&
						*(p->spins+xminus1+y*xlen)==*(p->spins+xminus2+y*xlen) && *(p->spins+xminus1+y*xlen)==*(p->spins+xminus1+yplus1*xlen) &&
						*(p->spins+x+yminus1*xlen)==*(p->spins+xplus1+yminus1*xlen) && *(p->spins+x+yminus1*xlen)==*(p->spins+x+yminus2*xlen) &&
						*(p->spins+xminus1+yminus1*xlen)==*(p->spins+xminus2+yminus1*xlen) && *(p->spins+xminus1+yminus1*xlen)==*(p->spins+xminus1+yminus2*xlen))
					{
						num_diff=0;

						if (*(p->spins+x+y*xlen)!=*(p->spins+xminus1+y*xlen)) num_diff++;
						if (*(p->spins+xminus1+y*xlen)!=*(p->spins+xminus1+yminus1*xlen)) num_diff++;
						if (*(p->spins+xminus1+yminus1*xlen)!=*(p->spins+x+yminus1*xlen)) num_diff++;
						if (*(p->spins+x+yminus1*xlen)!=*(p->spins+x+y*xlen)) num_diff++;

						if (num_diff==3) {*(p->T_vertices_2+x+y*xlen)=1; count++;} else *(p->T_vertices_2+x+y*xlen)=0;
					}
					else *(p->T_vertices_2+x+y*xlen)=0;
				}
			}

			*(final_T_vertices+(final_T_vertices_parameters->index))=(double)count;
			
			(final_T_vertices_parameters->index)++;
			final_T_vertices_parameters->size+=sizeof(double);

			return result_continue;
		}
		default: return result_continue;
	}
}

short int save_seed(short int command, struct save_seed_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct save_seed_data cmd_data;
	
	static double *seed_low;
	static double *seed_high;

	static unsigned long int num2pow32;

	static struct rng_struct *rng_struct;
	static int *rng_struct_size;
	
	static struct save_seed_parameters *seed_low_parameters;
	static struct save_seed_parameters *seed_high_parameters;

	switch (command)
	{
		case command_info: {printf("save_seed\n"); return result_continue;}
		case command_insert:
		{
			memcpy(&cmd_data, command_data, sizeof(struct save_seed_data));

			num2pow32=pow(2,16)*pow(2,16);

			return result_continue;
		}
		case command_begin:
		{
			if (data_input(data, data_parameters, &rng_struct, &rng_struct_size, cmd_data.data_index_rng, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &seed_low, &seed_low_parameters, cmd_data.size*sizeof(double), sizeof(struct save_seed_parameters), cmd_data.data_index_low, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &seed_high, &seed_high_parameters, cmd_data.size*sizeof(double), sizeof(struct save_seed_parameters), cmd_data.data_index_high, data_size)==1) return result_error;
			
			seed_low_parameters->index=0;
			seed_low_parameters->size=0;
			
			seed_high_parameters->index=0;
			seed_high_parameters->size=0;

			return result_continue;
		}
		case command_run:
		{
			*(seed_low+(seed_low_parameters->index))=(double)((rng_struct->seed)%num2pow32);
			*(seed_high+(seed_high_parameters->index))=(double)((rng_struct->seed)/num2pow32);
			
			(seed_low_parameters->index)++;
			(seed_high_parameters->index)++;
			
			seed_low_parameters->size+=sizeof(double);
			seed_high_parameters->size+=sizeof(double);

			return result_continue;
		}
		default: return result_continue;
	}
}

