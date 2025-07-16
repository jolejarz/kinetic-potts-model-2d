/**************************************************************
***************************************************************
*** 
***   step.c
***
***   Jason Olejarz
***   2011-01-27
***
***   This file contains the routines used by 'evolve' for intermediate processing.
***
***************************************************************
**************************************************************/

#include "includes.h"

short int step_check(short int command, struct step_check_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_check_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;

	static int last_energy;
	static int energy_after_bias;

	static double time_of_energy_drop;

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

	static short int same_energy_count;

	static short int bias;

	static short int eligible_spins;

	static short int run_force;

	static short int first_check_done;

	static int i, j, k;

	static short int q;

	switch (command)
	{
		case command_info: {printf("step_check\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_check_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			
			q=p->q;

			// Allocate memory.
			spins_backup=(short int*)malloc (sizeof(short int)*(p->xlen)*(p->ylen));
			state_backup=(short int*)malloc (sizeof(short int)*(p->xlen)*(p->ylen));
			for (i=0; i<11+MAX_Q*(MAX_Q-1); i++)
			{
				map_backup[i]=(int*)malloc (sizeof(int)*(p->xlen)*(p->ylen));
				index_backup[i]=(int*)malloc (sizeof(int)*(1+(p->xlen)*(p->ylen)));
			}
			T_vertices_backup=(short int*)malloc (sizeof(short int)*(p->xlen)*(p->ylen));
			T_vertices_2_backup=(short int*)malloc (sizeof(short int)*(p->xlen)*(p->ylen));

			return result_continue;
		}
		case command_begin:
		{
			last_energy=(int)(p->energy);

			first_check_done=0;

			return result_continue;
		}
		case command_run:
		{
			if ((int)(p->energy)<last_energy || first_check_done==0)
			{
				if ((int)(p->energy)<last_energy)
				{
					time_of_energy_drop=p->time;
					last_energy=(int)(p->energy);
				}

				if (p->time>=cmd_data.time)
				{
					first_check_done=1;

					if (*(p->index[3])==0 && *(p->index[4])==0 && *(p->index[6])==0 && *(p->index[7])==0 && *(p->index[8])==0 && *(p->index[9])==0 && *(p->index[10])==0)
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

						same_energy_count=0;

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
										data,
										data_parameters,
										data_size,
										p,
										0)
									==result_error) return result_error;

								// Restore the state of the random number generator.
								(p->rng_struct)->seed=rng_seed_backup;
								(p->rng_struct)->current_long_int=rng_long_int_backup;
								(p->rng_struct)->current_double=rng_double_backup;
								(p->rng_struct)->rng_restore();
							}

							energy_after_bias=(int)(p->energy);

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

							if (energy_after_bias==last_energy) same_energy_count++;
							else break;
						}

						// If the energy does not drop after application of all bias fields,
						// then the system is in its lowest energy state.
						if (same_energy_count==2*q)
						{
							p->time=time_of_energy_drop;
							return result_done;
						}
					}
				}
			}
			
			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_check_2(short int command, struct step_check_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_check_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;

	static int last_energy;
	static int energy_after_bias;

	static double time_of_energy_drop;

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

	static short int same_energy_count;

	static short int bias;

	static short int eligible_spins;

	static short int run_force;

	static short int first_check_done;

	static int i, j, k;

	static short int q;

	switch (command)
	{
		case command_info: {printf("step_check_2\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_check_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			
			q=p->q;

			// Allocate memory.
			spins_backup=(short int*)malloc (sizeof(short int)*(p->xlen)*(p->ylen));
			state_backup=(short int*)malloc (sizeof(short int)*(p->xlen)*(p->ylen));
			for (i=0; i<11+MAX_Q*(MAX_Q-1); i++)
			{
				map_backup[i]=(int*)malloc (sizeof(int)*(p->xlen)*(p->ylen));
				index_backup[i]=(int*)malloc (sizeof(int)*(1+(p->xlen)*(p->ylen)));
			}
			T_vertices_backup=(short int*)malloc (sizeof(short int)*(p->xlen)*(p->ylen));
			T_vertices_2_backup=(short int*)malloc (sizeof(short int)*(p->xlen)*(p->ylen));

			return result_continue;
		}
		case command_begin:
		{
			last_energy=(int)(p->energy);

			first_check_done=0;

			return result_continue;
		}
		case command_run:
		{
			if ((int)(p->energy)<last_energy || first_check_done==0)
			{
				if ((int)(p->energy)<last_energy)
				{
					time_of_energy_drop=p->time;
					last_energy=(int)(p->energy);
				}

				if (p->time>=cmd_data.time)
				{
					first_check_done=1;

					if (*(p->index[3])==0 && *(p->index[4])==0 && *(p->index[6])==0 && *(p->index[7])==0 && *(p->index[8])==0 && *(p->index[9])==0 && *(p->index[10])==0)
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

						same_energy_count=0;

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
								/*
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
								*/
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
										data,
										data_parameters,
										data_size,
										p,
										0)
									==result_error) return result_error;

								// Restore the state of the random number generator.
								(p->rng_struct)->seed=rng_seed_backup;
								(p->rng_struct)->current_long_int=rng_long_int_backup;
								(p->rng_struct)->current_double=rng_double_backup;
								(p->rng_struct)->rng_restore();
							}

							energy_after_bias=(int)(p->energy);

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

							if (energy_after_bias==last_energy) same_energy_count++;
							else break;
						}

						// If the energy does not drop after application of all bias fields,
						// then the system is in its lowest energy state.
						if (same_energy_count==2*q)
						{
							p->time=time_of_energy_drop;
							return result_done;
						}
					}
				}
			}
			
			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_energy(short int command, struct step_energy_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_energy_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	static double *energy;
	static struct step_energy_parameters *energy_parameters;
	
	switch (command)
	{
		case command_info: {printf("step_energy\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_energy_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &trigger, &trigger_parameters, cmd_data.data_index_trigger, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &energy, &energy_parameters, sizeof(double)*((trigger_parameters->total)+1), sizeof(struct step_energy_parameters), cmd_data.data_index_energy, data_size)==1) return result_error;
			
			energy_parameters->size=0;
			energy_parameters->index=0;

			return result_continue;
		}
		case command_begin:
		{
			*(energy+(energy_parameters->index))=p->energy;
			
			(energy_parameters->size)+=sizeof(double);
			(energy_parameters->index)++;

			return result_continue;
		}
		case command_run:
		{
			if (*trigger==1)
			{
				*(energy+(energy_parameters->index))=p->energy;
				
				(energy_parameters->size)+=sizeof(double);
				(energy_parameters->index)++;
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_energy_2(short int command, struct step_energy_2_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_energy_2_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static double *energy;
	static struct step_energy_2_parameters *energy_parameters;
	
	switch (command)
	{
		case command_info: {printf("step_energy_2\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_energy_2_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &energy, &energy_parameters, (int)(sizeof(double)*(1+(cmd_data.ending_step)-(cmd_data.starting_step))), sizeof(struct step_energy_2_parameters), cmd_data.data_index_energy, data_size)==1) return result_error;

			energy_parameters->size=0;
			energy_parameters->index=0;

			return result_continue;
		}
		case command_begin:
		{
			if (p->steps>=cmd_data.starting_step && p->steps<=cmd_data.ending_step)
			{
				*(energy+(energy_parameters->index)++)=p->energy;
				energy_parameters->size+=sizeof(double);
			}

			return result_continue;
		}
		case command_run:
		{
			if (p->steps>=cmd_data.starting_step && p->steps<=cmd_data.ending_step)
			{
				*(energy+(energy_parameters->index)++)=p->energy;
				energy_parameters->size+=sizeof(double);
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_energy_average(short int command, struct step_energy_average_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_energy_average_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;

	static double *energy;
	static struct step_energy_average_parameters *energy_parameters;
	static double *runs;
	static struct step_energy_average_parameters *runs_parameters;

	static unsigned long int *total_energy;

	static int num_energy_values;

	static int i;

	static double last_time;
	
	switch (command)
	{
		case command_info: {printf("step_energy_average\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_energy_average_data));

			num_energy_values=(int)(1+(((cmd_data.ending_time)-(cmd_data.starting_time))/(cmd_data.time_interval)));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &energy, &energy_parameters, sizeof(double)*num_energy_values, sizeof(struct step_energy_average_parameters), cmd_data.data_index_energy, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &runs, &runs_parameters, sizeof(double)*num_energy_values, sizeof(struct step_energy_average_parameters), cmd_data.data_index_runs, data_size)==1) return result_error;

			total_energy=(unsigned long int*) malloc (sizeof(unsigned long int)*num_energy_values);

			for (i=0; i<num_energy_values; i++)
			{
				*(total_energy+i)=0;
				*(energy+i)=0;
				*(runs+i)=0;
			}

			return result_continue;
		}
		case command_begin:
		{
			energy_parameters->size=0;
			energy_parameters->index=0;

			runs_parameters->size=0;
			runs_parameters->index=0;

			(*(runs+(runs_parameters->index)))++;
			runs_parameters->size+=sizeof(double);

			*(total_energy+(energy_parameters->index))+=(unsigned long int)p->energy;

			*(energy+(energy_parameters->index))=*(total_energy+(energy_parameters->index))/(*(runs+(runs_parameters->index)));
			energy_parameters->size+=sizeof(double);

			(runs_parameters->index)++;
			(energy_parameters->index)++;

			last_time=0;

			return result_continue;
		}
		case command_run:
		{
			if ((p->time)-last_time>=cmd_data.time_interval)
			{
				(*(runs+(runs_parameters->index)))++;
				runs_parameters->size+=sizeof(double);

				*(total_energy+(energy_parameters->index))+=(unsigned long int)p->energy;

				*(energy+(energy_parameters->index))=*(total_energy+(energy_parameters->index))/(*(runs+(runs_parameters->index)));
				energy_parameters->size+=sizeof(double);

				(runs_parameters->index)++;
				(energy_parameters->index)++;

				last_time+=cmd_data.time_interval;
			}

			return result_continue;
		}
		case command_end:
		{
			while ((energy_parameters->index)<num_energy_values)
			{
				(*(runs+(runs_parameters->index)))++;
				runs_parameters->size+=sizeof(double);

				*(total_energy+(energy_parameters->index))+=(unsigned long int)p->energy;

				*(energy+(energy_parameters->index))=*(total_energy+(energy_parameters->index))/(*(runs+(runs_parameters->index)));
				energy_parameters->size+=sizeof(double);

				(runs_parameters->index)++;
				(energy_parameters->index)++;
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_T_vertices_average(short int command, struct step_T_vertices_average_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_T_vertices_average_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;

	static double *T_vertices;
	static struct step_T_vertices_average_parameters *T_vertices_parameters;
	static double *runs;
	static struct step_T_vertices_average_parameters *runs_parameters;

	static unsigned long int *total_T_vertices;

	static int num_T_vertices_values;

	static int i;

	static double last_time;
	
	static int count, num_diff;
	static int x, y, xlen, ylen;
	static int xminus1, xminus2, xminus3, xplus1, xplus2, xplus3;
	static int yminus1, yminus2, yminus3, yplus1, yplus2, yplus3;

	static short int check;
	static int x_flipped, y_flipped;

	switch (command)
	{
		case command_info: {printf("step_T_vertices_average\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_T_vertices_average_data));

			num_T_vertices_values=(int)(1+(((cmd_data.ending_time)-(cmd_data.starting_time))/(cmd_data.time_interval)));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &T_vertices, &T_vertices_parameters, sizeof(double)*num_T_vertices_values, sizeof(struct step_T_vertices_average_parameters), cmd_data.data_index_T_vertices, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &runs, &runs_parameters, sizeof(double)*num_T_vertices_values, sizeof(struct step_T_vertices_average_parameters), cmd_data.data_index_runs, data_size)==1) return result_error;

			total_T_vertices=(unsigned long int*) malloc (sizeof(unsigned long int)*num_T_vertices_values);

			for (i=0; i<num_T_vertices_values; i++)
			{
				*(total_T_vertices+i)=0;
				*(T_vertices+i)=0;
				*(runs+i)=0;
			}

			return result_continue;
		}
		case command_begin:
		{
			xlen=p->xlen;
			ylen=p->ylen;

			// Find and mark all T vertices in the system.
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

			T_vertices_parameters->size=0;
			T_vertices_parameters->index=0;

			runs_parameters->size=0;
			runs_parameters->index=0;

			(*(runs+(runs_parameters->index)))++;
			runs_parameters->size+=sizeof(double);

			*(total_T_vertices+(T_vertices_parameters->index))+=(unsigned long int)count;

			*(T_vertices+(T_vertices_parameters->index))=*(total_T_vertices+(T_vertices_parameters->index))/(*(runs+(runs_parameters->index)));
			T_vertices_parameters->size+=sizeof(double);

			(runs_parameters->index)++;
			(T_vertices_parameters->index)++;

			last_time=0;

			return result_continue;
		}
		case command_run:
		{
			// Get the x and y coordinates of the flipped spin.
			y_flipped=(int)((p->coordinate)/xlen);
			x_flipped=(int)((p->coordinate)-y_flipped*xlen);

			x=x_flipped; y=y_flipped;

			// Check if any new T vertices were created.
			for (check=0; check<4; check++)
			{
				switch (check)
				{
					case 0: {x=x_flipped; y=y_flipped; break;}
					case 1: {if (x_flipped==xlen-1) x=0; else x=x_flipped+1; y=y_flipped; break;}
					case 2: {x=x_flipped; if (y_flipped==ylen-1) y=0; else y=y_flipped+1; break;}
					case 3: {if (x_flipped==xlen-1) x=0; else x=x_flipped+1; if (y_flipped==ylen-1) y=0; else y=y_flipped+1; break;}
				}

				// Determine the locations of the neighboring spins.
				if (x==0) xminus1=xlen-1; else xminus1=x-1;
				if (y==0) yminus1=ylen-1; else yminus1=y-1;

				num_diff=0;

				if (*(p->spins+x+y*xlen)!=*(p->spins+xminus1+y*xlen)) num_diff++;
				if (*(p->spins+xminus1+y*xlen)!=*(p->spins+xminus1+yminus1*xlen)) num_diff++;
				if (*(p->spins+xminus1+yminus1*xlen)!=*(p->spins+x+yminus1*xlen)) num_diff++;
				if (*(p->spins+x+yminus1*xlen)!=*(p->spins+x+y*xlen)) num_diff++;

				if (num_diff==3)
				{
					if (*(p->T_vertices+x+y*xlen)==0)
					{
						*(p->T_vertices+x+y*xlen)=1;
						count++;
					}
				}
				else
				{
					if (*(p->T_vertices+x+y*xlen)==1)
					{
						*(p->T_vertices+x+y*xlen)=0;
						count--;
					}
				}
			}

			if ((p->time)-last_time>=cmd_data.time_interval)
			{
				(*(runs+(runs_parameters->index)))++;
				runs_parameters->size+=sizeof(double);

				*(total_T_vertices+(T_vertices_parameters->index))+=(unsigned long int)count;

				*(T_vertices+(T_vertices_parameters->index))=*(total_T_vertices+(T_vertices_parameters->index))/(*(runs+(runs_parameters->index)));
				T_vertices_parameters->size+=sizeof(double);

				(runs_parameters->index)++;
				(T_vertices_parameters->index)++;

				last_time+=cmd_data.time_interval;
			}

			return result_continue;
		}
		case command_end:
		{
			while ((T_vertices_parameters->index)<num_T_vertices_values)
			{
				(*(runs+(runs_parameters->index)))++;
				runs_parameters->size+=sizeof(double);

				*(total_T_vertices+(T_vertices_parameters->index))+=(unsigned long int)count;

				*(T_vertices+(T_vertices_parameters->index))=*(total_T_vertices+(T_vertices_parameters->index))/(*(runs+(runs_parameters->index)));
				T_vertices_parameters->size+=sizeof(double);

				(runs_parameters->index)++;
				(T_vertices_parameters->index)++;
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_T_vertices_2_average(short int command, struct step_T_vertices_average_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_T_vertices_average_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;

	static double *T_vertices;
	static struct step_T_vertices_average_parameters *T_vertices_parameters;
	static double *runs;
	static struct step_T_vertices_average_parameters *runs_parameters;

	static unsigned long int *total_T_vertices;

	static int num_T_vertices_values;

	static int i;

	static double last_time;
	
	static int count, num_diff;
	static int x, y, xlen, ylen;
	static int xminus1, xminus2, xminus3, xplus1, xplus2, xplus3;
	static int yminus1, yminus2, yminus3, yplus1, yplus2, yplus3;

	static short int check;
	static int x_flipped, y_flipped;

	switch (command)
	{
		case command_info: {printf("step_T_vertices_average\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_T_vertices_average_data));

			num_T_vertices_values=(int)(1+(((cmd_data.ending_time)-(cmd_data.starting_time))/(cmd_data.time_interval)));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &T_vertices, &T_vertices_parameters, sizeof(double)*num_T_vertices_values, sizeof(struct step_T_vertices_average_parameters), cmd_data.data_index_T_vertices, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &runs, &runs_parameters, sizeof(double)*num_T_vertices_values, sizeof(struct step_T_vertices_average_parameters), cmd_data.data_index_runs, data_size)==1) return result_error;

			total_T_vertices=(unsigned long int*) malloc (sizeof(unsigned long int)*num_T_vertices_values);

			for (i=0; i<num_T_vertices_values; i++)
			{
				*(total_T_vertices+i)=0;
				*(T_vertices+i)=0;
				*(runs+i)=0;
			}

			return result_continue;
		}
		case command_begin:
		{
			xlen=p->xlen;
			ylen=p->ylen;

			// Find and mark all T vertices in the system.
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

			T_vertices_parameters->size=0;
			T_vertices_parameters->index=0;

			runs_parameters->size=0;
			runs_parameters->index=0;

			(*(runs+(runs_parameters->index)))++;
			runs_parameters->size+=sizeof(double);

			*(total_T_vertices+(T_vertices_parameters->index))+=(unsigned long int)count;

			*(T_vertices+(T_vertices_parameters->index))=*(total_T_vertices+(T_vertices_parameters->index))/(*(runs+(runs_parameters->index)));
			T_vertices_parameters->size+=sizeof(double);

			(runs_parameters->index)++;
			(T_vertices_parameters->index)++;

			last_time=0;

			return result_continue;
		}
		case command_run:
		{
			// Get the x and y coordinates of the flipped spin.
			y_flipped=(int)((p->coordinate)/xlen);
			x_flipped=(int)((p->coordinate)-y_flipped*xlen);

			x=x_flipped; y=y_flipped;

			// Check if any new T vertices were created.
			for (check=0; check<12; check++)
			{
				switch (check)
				{
					case 0: {x=x_flipped; y=y_flipped; break;}
					case 1: {if (x_flipped==0) x=xlen-1; else x=x_flipped-1; y=y_flipped; break;}
					case 2: {x=x_flipped; if (y_flipped==0) y=ylen-1; else y=y_flipped-1; break;}
					case 3: {if (x_flipped==xlen-1) x=0; else x=x_flipped+1; y=y_flipped; break;}
					case 4: {if (x_flipped==xlen-1) x=1; else if (x_flipped==xlen-2) x=0; else x=x_flipped+2; y=y_flipped; break;}
					case 5: {if (x_flipped==xlen-1) x=0; else x=x_flipped+1; if (y_flipped==0) y=ylen-1; else y=y_flipped-1; break;}
					case 6: {x=x_flipped; if (y_flipped==ylen-1) y=0; else y=y_flipped+1; break;}
					case 7: {if (x_flipped==0) x=xlen-1; else x=x_flipped-1; if (y_flipped==ylen-1) y=0; else y=y_flipped+1; break;}
					case 8: {x=x_flipped; if (y_flipped==ylen-1) y=1; else if (y_flipped==ylen-2) y=0; else y=y_flipped+2; break;}
					case 9: {if (x_flipped==xlen-1) x=0; else x=x_flipped+1; if (y_flipped==ylen-1) y=0; else y=y_flipped+1; break;}
					case 10: {if (x_flipped==xlen-1) x=1; else if (x_flipped==xlen-2) x=0; else x=x_flipped+2; if (y_flipped==ylen-1) y=0; else y=y_flipped+1; break;}
					case 11: {if (x_flipped==xlen-1) x=0; else x=x_flipped+1; if (y_flipped==ylen-1) y=1; else if (y_flipped==ylen-2) y=0; else y=y_flipped+2; break;}
				}

				// Determine the locations of the neighboring spins.
				if (x==0) xminus1=xlen-1; else xminus1=x-1;
				if (x==0) xminus2=xlen-2; else xminus2=x-2;
				if (x==1) xminus2=xlen-1; else xminus2=x-2;
				if (x==xlen-1) xplus1=0; else xplus1=x+1;
				if (y==0) yminus1=ylen-1; else yminus1=y-1;
				if (y==0) yminus2=ylen-2; else yminus2=y-2;
				if (y==1) yminus2=ylen-1; else yminus2=y-2;
				if (y==ylen-1) yplus1=0; else yplus1=y+1;

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

					if (num_diff==3)
					{
						if (*(p->T_vertices_2+x+y*xlen)==0)
						{
							*(p->T_vertices_2+x+y*xlen)=1;
							count++;
						}
					}
					else
					{
						if (*(p->T_vertices_2+x+y*xlen)==1)
						{
							*(p->T_vertices_2+x+y*xlen)=0;
							count--;
						}
					}
				}
				else if (*(p->T_vertices_2+x+y*xlen)==1)
				{
					*(p->T_vertices_2+x+y*xlen)=0;
					count--;
				}
			}

			if ((p->time)-last_time>=cmd_data.time_interval)
			{
				(*(runs+(runs_parameters->index)))++;
				runs_parameters->size+=sizeof(double);

				*(total_T_vertices+(T_vertices_parameters->index))+=(unsigned long int)count;

				*(T_vertices+(T_vertices_parameters->index))=*(total_T_vertices+(T_vertices_parameters->index))/(*(runs+(runs_parameters->index)));
				T_vertices_parameters->size+=sizeof(double);

				(runs_parameters->index)++;
				(T_vertices_parameters->index)++;

				last_time+=cmd_data.time_interval;
			}

			return result_continue;
		}
		case command_end:
		{
			while ((T_vertices_parameters->index)<num_T_vertices_values)
			{
				(*(runs+(runs_parameters->index)))++;
				runs_parameters->size+=sizeof(double);

				*(total_T_vertices+(T_vertices_parameters->index))+=(unsigned long int)count;

				*(T_vertices+(T_vertices_parameters->index))=*(total_T_vertices+(T_vertices_parameters->index))/(*(runs+(runs_parameters->index)));
				T_vertices_parameters->size+=sizeof(double);

				(runs_parameters->index)++;
				(T_vertices_parameters->index)++;
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_flips(short int command, struct step_flips_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_flips_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static int *flips;
	static struct step_flips_parameters *flips_parameters;
	
	switch (command)
	{
		case command_info: {printf("step_flips\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_flips_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &flips, &flips_parameters, (int)(sizeof(int)*2*(p->maxsteps)), sizeof(struct step_flips_parameters), cmd_data.data_index_flips, data_size)==1) return result_error;

			flips_parameters->size=0;
			flips_parameters->index=0;

			return result_continue;
		}
		case command_run:
		{
			*(flips+(flips_parameters->index)++)=p->coordinate;
			*(flips+(flips_parameters->index)++)=p->move;

			flips_parameters->size+=2*sizeof(int);

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_flips_2(short int command, struct step_flips_2_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_flips_2_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static int *flips;
	static struct step_flips_2_parameters *flips_parameters;
	
	switch (command)
	{
		case command_info: {printf("step_flips_2\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_flips_2_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &flips, &flips_parameters, (int)(sizeof(int)*2*((cmd_data.ending_step)-(cmd_data.starting_step))), sizeof(struct step_flips_2_parameters), cmd_data.data_index_flips, data_size)==1) return result_error;

			flips_parameters->size=0;
			flips_parameters->index=0;

			return result_continue;
		}
		case command_run:
		{
			if (p->steps>cmd_data.starting_step && p->steps<=cmd_data.ending_step)
			{
				*(flips+(flips_parameters->index)++)=p->coordinate;
				*(flips+(flips_parameters->index)++)=p->move;
				flips_parameters->size+=2*sizeof(int);
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_flips_backward_2(short int command, struct step_flips_backward_2_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_flips_backward_2_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static int *flips_backward;
	static struct step_flips_backward_2_parameters *flips_backward_parameters;
	
	switch (command)
	{
		case command_info: {printf("step_flips_backward_2\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_flips_backward_2_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &flips_backward, &flips_backward_parameters, (int)(sizeof(int)*2*((cmd_data.ending_step)-(cmd_data.starting_step))), sizeof(struct step_flips_backward_2_parameters), cmd_data.data_index_flips_backward, data_size)==1) return result_error;

			flips_backward_parameters->size=0;
			flips_backward_parameters->index=0;

			return result_continue;
		}
		case command_run:
		{
			if (p->steps>cmd_data.starting_step && p->steps<=cmd_data.ending_step)
			{
				*(flips_backward+(flips_backward_parameters->index)++)=p->coordinate;
				*(flips_backward+(flips_backward_parameters->index)++)=p->move_backward;
				flips_backward_parameters->size+=2*sizeof(int);
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_force(short int command, struct step_force_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_force_data cmd_data;
	
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

	switch (command)
	{
		case command_info: {printf("step_force\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_force_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			
			q=p->q;

			return result_continue;
		}
		case command_begin:
		{
			return result_continue;
		}
		case command_run:
		{
			if (p->time>=cmd_data.time)
			{
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

						return result_done;
					}

					// Change the bias field.
					if (bias<2*q-1) bias++; else bias=0;
				}
			}
		}
		default: return result_continue;
	}
}

short int step_force_2(short int command, struct step_force2_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_force2_data cmd_data;
	
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
		case command_info: {printf("step_force_2\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_force2_data));

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
			// Initialize these variables.
			energy_0_value=p->energy;
			energy_1_value=0;
			num_energy_drops_1_value=0;
			num_energy_drops_2_value=0;

			return result_continue;
		}
		case command_run:
		{
			num_energy_drops_1_value=p->num_energy_drops;

			if (p->time>=cmd_data.time)
			{
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

						num_energy_drops_2_value+=p->num_energy_drops;
					}

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

						return result_done;
					}

					// Change the bias field.
					if (bias<2*q-1) bias++; else bias=0;
				}
			}
			return result_continue;
		}
		case command_end:
		{
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
		default: return result_continue;
	}
}

short int step_mag(short int command, struct step_mag_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_mag_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	static double *mag;
	static struct step_mag_parameters *mag_parameters;
	
	switch (command)
	{
		case command_info: {printf("step_mag\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_mag_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &trigger, &trigger_parameters, cmd_data.data_index_trigger, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &mag, &mag_parameters, sizeof(double)*((trigger_parameters->total)+1), sizeof(struct step_mag_parameters), cmd_data.data_index_mag, data_size)==1) return result_error;
			
			mag_parameters->size=0;
			mag_parameters->index=0;

			return result_continue;
		}
		case command_begin:
		{
			*(mag+(mag_parameters->index))=(double)(p->mag);
			
			(mag_parameters->size)+=sizeof(double);
			(mag_parameters->index)++;

			return result_continue;
		}
		case command_run:
		{
			if (*trigger==1)
			{
				*(mag+(mag_parameters->index))=(double)(p->mag);
				
				(mag_parameters->size)+=sizeof(double);
				(mag_parameters->index)++;
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_save_lattice(short int command, struct step_save_lattice_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_save_lattice_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;

	static short int *spins;
	static int *spins_size;
	
	static FILE *data_file, *plot_file;

	switch (command)
	{
		case command_info: {printf("step_save_lattice\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_save_lattice_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &spins, &spins_size, sizeof(short int)*(p->xlen)*(p->ylen), sizeof(struct step_save_lattice_parameters), cmd_data.data_index_spins, data_size)==1) return result_error;
			
			return result_continue;
		}
		case command_begin:
		{
			if (p->steps==cmd_data.plot_step)
			{
				data_file=fopen(cmd_data.matlab_data_filename, "w");
				plot_file=fopen(cmd_data.matlab_plot_filename, "w");

				state_plot_matlab(p->spins, p->xlen, p->ylen, 0, cmd_data.matlab_data_filename, data_file, plot_file);
				
				fclose(plot_file);
				fclose(data_file);

				memcpy (spins, p->spins, sizeof(short int)*(p->xlen)*(p->ylen));
				*spins_size=sizeof(short int)*(p->xlen)*(p->ylen);
			}

			return result_continue;
		}
		case command_run:
		{
			if (p->steps==cmd_data.plot_step)
			{
				data_file=fopen(cmd_data.matlab_data_filename, "w");
				plot_file=fopen(cmd_data.matlab_plot_filename, "w");

				state_plot_matlab(p->spins, p->xlen, p->ylen, 0, cmd_data.matlab_data_filename, data_file, plot_file);
				
				fclose(plot_file);
				fclose(data_file);

				memcpy (spins, p->spins, sizeof(short int)*(p->xlen)*(p->ylen));
				*spins_size=sizeof(short int)*(p->xlen)*(p->ylen);
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_T_vertices(short int command, struct step_T_vertices_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_T_vertices_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	static double *T_vertices;
	static struct step_T_vertices_parameters *T_vertices_parameters;
	
	static int count, num_diff;
	static int x, y, xlen, ylen;
	static int xminus1, xminus2, xminus3, xplus1, xplus2, xplus3;
	static int yminus1, yminus2, yminus3, yplus1, yplus2, yplus3;

	static short int check;
	static int x_flipped, y_flipped;

	switch (command)
	{
		case command_info: {printf("step_T_vertices\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_T_vertices_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &trigger, &trigger_parameters, cmd_data.data_index_trigger, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &T_vertices, &T_vertices_parameters, sizeof(double)*((trigger_parameters->total)+1), sizeof(struct step_T_vertices_parameters), cmd_data.data_index_T_vertices, data_size)==1) return result_error;
			
			T_vertices_parameters->size=0;
			T_vertices_parameters->index=0;

			return result_continue;
		}
		case command_begin:
		{
			xlen=p->xlen;
			ylen=p->ylen;

			// Find and mark all T vertices in the system.
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

			*(T_vertices+(T_vertices_parameters->index))=(double)count;
			
			(T_vertices_parameters->size)+=sizeof(double);
			(T_vertices_parameters->index)++;

			return result_continue;
		}
		case command_run:
		{
			// Get the x and y coordinates of the flipped spin.
			y_flipped=(int)((p->coordinate)/xlen);
			x_flipped=(int)((p->coordinate)-y_flipped*xlen);

			x=x_flipped; y=y_flipped;

			// Check if any new T vertices were created.
			for (check=0; check<4; check++)
			{
				switch (check)
				{
					case 0: {x=x_flipped; y=y_flipped; break;}
					case 1: {if (x_flipped==xlen-1) x=0; else x=x_flipped+1; y=y_flipped; break;}
					case 2: {x=x_flipped; if (y_flipped==ylen-1) y=0; else y=y_flipped+1; break;}
					case 3: {if (x_flipped==xlen-1) x=0; else x=x_flipped+1; if (y_flipped==ylen-1) y=0; else y=y_flipped+1; break;}
				}

				// Determine the locations of the neighboring spins.
				if (x==0) xminus1=xlen-1; else xminus1=x-1;
				if (y==0) yminus1=ylen-1; else yminus1=y-1;

				num_diff=0;

				if (*(p->spins+x+y*xlen)!=*(p->spins+xminus1+y*xlen)) num_diff++;
				if (*(p->spins+xminus1+y*xlen)!=*(p->spins+xminus1+yminus1*xlen)) num_diff++;
				if (*(p->spins+xminus1+yminus1*xlen)!=*(p->spins+x+yminus1*xlen)) num_diff++;
				if (*(p->spins+x+yminus1*xlen)!=*(p->spins+x+y*xlen)) num_diff++;

				if (num_diff==3)
				{
					if (*(p->T_vertices+x+y*xlen)==0)
					{
						*(p->T_vertices+x+y*xlen)=1;
						count++;
					}
				}
				else
				{
					if (*(p->T_vertices+x+y*xlen)==1)
					{
						*(p->T_vertices+x+y*xlen)=0;
						count--;
					}
				}
			}

			if (*trigger==1)
			{
				*(T_vertices+(T_vertices_parameters->index))=(double)count;
				
				(T_vertices_parameters->size)+=sizeof(double);
				(T_vertices_parameters->index)++;
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_T_vertices_2(short int command, struct step_T_vertices_2_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_T_vertices_2_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	static double *T_vertices;
	static struct step_T_vertices_2_parameters *T_vertices_parameters;
	
	static int count, num_diff;
	static int x, y, xlen, ylen;
	static int xminus1, xminus2, xminus3, xplus1, xplus2, xplus3;
	static int yminus1, yminus2, yminus3, yplus1, yplus2, yplus3;

	static short int check;
	static int x_flipped, y_flipped;

	switch (command)
	{
		case command_info: {printf("step_T_vertices_2\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_T_vertices_2_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &trigger, &trigger_parameters, cmd_data.data_index_trigger, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &T_vertices, &T_vertices_parameters, sizeof(double)*((trigger_parameters->total)+1), sizeof(struct step_T_vertices_2_parameters), cmd_data.data_index_T_vertices, data_size)==1) return result_error;
			
			T_vertices_parameters->size=0;
			T_vertices_parameters->index=0;

			return result_continue;
		}
		case command_begin:
		{
			xlen=p->xlen;
			ylen=p->ylen;

			// Find and mark all T vertices in the system.
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

			*(T_vertices+(T_vertices_parameters->index))=(double)count;
			
			(T_vertices_parameters->size)+=sizeof(double);
			(T_vertices_parameters->index)++;

			return result_continue;
		}
		case command_run:
		{
			// Get the x and y coordinates of the flipped spin.
			y_flipped=(int)((p->coordinate)/xlen);
			x_flipped=(int)((p->coordinate)-y_flipped*xlen);

			x=x_flipped; y=y_flipped;

			// Check if any new T vertices were created.
			for (check=0; check<12; check++)
			{
				switch (check)
				{
					case 0: {x=x_flipped; y=y_flipped; break;}
					case 1: {if (x_flipped==0) x=xlen-1; else x=x_flipped-1; y=y_flipped; break;}
					case 2: {x=x_flipped; if (y_flipped==0) y=ylen-1; else y=y_flipped-1; break;}
					case 3: {if (x_flipped==xlen-1) x=0; else x=x_flipped+1; y=y_flipped; break;}
					case 4: {if (x_flipped==xlen-1) x=1; else if (x_flipped==xlen-2) x=0; else x=x_flipped+2; y=y_flipped; break;}
					case 5: {if (x_flipped==xlen-1) x=0; else x=x_flipped+1; if (y_flipped==0) y=ylen-1; else y=y_flipped-1; break;}
					case 6: {x=x_flipped; if (y_flipped==ylen-1) y=0; else y=y_flipped+1; break;}
					case 7: {if (x_flipped==0) x=xlen-1; else x=x_flipped-1; if (y_flipped==ylen-1) y=0; else y=y_flipped+1; break;}
					case 8: {x=x_flipped; if (y_flipped==ylen-1) y=1; else if (y_flipped==ylen-2) y=0; else y=y_flipped+2; break;}
					case 9: {if (x_flipped==xlen-1) x=0; else x=x_flipped+1; if (y_flipped==ylen-1) y=0; else y=y_flipped+1; break;}
					case 10: {if (x_flipped==xlen-1) x=1; else if (x_flipped==xlen-2) x=0; else x=x_flipped+2; if (y_flipped==ylen-1) y=0; else y=y_flipped+1; break;}
					case 11: {if (x_flipped==xlen-1) x=0; else x=x_flipped+1; if (y_flipped==ylen-1) y=1; else if (y_flipped==ylen-2) y=0; else y=y_flipped+2; break;}
				}

				// Determine the locations of the neighboring spins.
				if (x==0) xminus1=xlen-1; else xminus1=x-1;
				if (x==0) xminus2=xlen-2; else xminus2=x-2;
				if (x==1) xminus2=xlen-1; else xminus2=x-2;
				if (x==xlen-1) xplus1=0; else xplus1=x+1;
				if (y==0) yminus1=ylen-1; else yminus1=y-1;
				if (y==0) yminus2=ylen-2; else yminus2=y-2;
				if (y==1) yminus2=ylen-1; else yminus2=y-2;
				if (y==ylen-1) yplus1=0; else yplus1=y+1;

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

					if (num_diff==3)
					{
						if (*(p->T_vertices_2+x+y*xlen)==0)
						{
							*(p->T_vertices_2+x+y*xlen)=1;
							count++;
						}
					}
					else
					{
						if (*(p->T_vertices_2+x+y*xlen)==1)
						{
							*(p->T_vertices_2+x+y*xlen)=0;
							count--;
						}
					}
				}
				else if (*(p->T_vertices_2+x+y*xlen)==1)
				{
					*(p->T_vertices_2+x+y*xlen)=0;
					count--;
				}
			}

			if (*trigger==1)
			{
				*(T_vertices+(T_vertices_parameters->index))=(double)count;
				
				(T_vertices_parameters->size)+=sizeof(double);
				(T_vertices_parameters->index)++;
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_time(short int command, struct step_time_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_time_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	static double *time;
	static struct step_time_parameters *time_parameters;
	
	switch (command)
	{
		case command_info: {printf("step_time\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_time_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &trigger, &trigger_parameters, cmd_data.data_index_trigger, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &time, &time_parameters, sizeof(double)*((trigger_parameters->total)+1), sizeof(struct step_time_parameters), cmd_data.data_index_time, data_size)==1) return result_error;
			
			time_parameters->size=0;
			time_parameters->index=0;

			return result_continue;
		}
		case command_begin:
		{
			*(time+(time_parameters->index))=p->time;
			
			(time_parameters->size)+=sizeof(double);
			(time_parameters->index)++;

			return result_continue;
		}
		case command_run:
		{
			if (*trigger==1)
			{
				*(time+(time_parameters->index))=p->time;
				
				(time_parameters->size)+=sizeof(double);
				(time_parameters->index)++;
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_time_2(short int command, struct step_time_2_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_time_2_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static double *time;
	static struct step_time_2_parameters *time_parameters;
	
	switch (command)
	{
		case command_info: {printf("step_time_2\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_time_2_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &time, &time_parameters, (int)(sizeof(double)*(1+(cmd_data.ending_step)-(cmd_data.starting_step))), sizeof(struct step_time_2_parameters), cmd_data.data_index_time, data_size)==1) return result_error;

			time_parameters->size=0;
			time_parameters->index=0;

			return result_continue;
		}
		case command_begin:
		{
			if (p->steps>=cmd_data.starting_step && p->steps<=cmd_data.ending_step)
			{
				*(time+(time_parameters->index)++)=p->time;
				time_parameters->size+=sizeof(double);
			}

			return result_continue;
		}
		case command_run:
		{
			if (p->steps>=cmd_data.starting_step && p->steps<=cmd_data.ending_step)
			{
				*(time+(time_parameters->index)++)=p->time;
				time_parameters->size+=sizeof(double);
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_trigger_steps(short int command, struct step_trigger_steps_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_trigger_steps_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	
	switch (command)
	{
		case command_begin:
		{
			if (cmd_data.progress==1)
			{
				printf("Processing...      ");
				fflush(stdout);
			}
			return result_continue;
		}
		case command_end:
		{
			if (cmd_data.progress==1)
			{
				printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\bDone                    \n");
				fflush(stdout);
			}
			return result_continue;
		}
		case command_info: {printf("step_trigger_steps\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_trigger_steps_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &trigger, &trigger_parameters, sizeof(short int), sizeof(struct step_trigger_parameters), cmd_data.data_index_trigger, data_size)==1) return result_error;
			
			trigger_parameters->total=(p->maxsteps)/cmd_data.steps_interval;
			trigger_parameters->current=0;
			trigger_parameters->steps=0;
			
			return result_continue;
		}
		case command_run:
		{
			if ((p->steps)%cmd_data.steps_interval==0)
			{
				*trigger=1;
				(trigger_parameters->current)++;
				trigger_parameters->steps=p->steps;

				if (cmd_data.progress==1)
				{
					printf("\b\b\b\b\b");
					printf("%4.1lf%%", 100*(trigger_parameters->current)/(double)(trigger_parameters->total));
					fflush(stdout);
				}
			}
			else *trigger=0;

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_trigger_time(short int command, struct step_trigger_time_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_trigger_time_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	
	static double time_elapsed;
	
	switch (command)
	{
		case command_begin:
		{
			if (cmd_data.progress==1)
			{
				printf("Processing...      ");
				fflush(stdout);
			}
			return result_continue;
		}
		case command_end:
		{
			if (cmd_data.progress==1)
			{
				printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\bDone                    \n");
				fflush(stdout);
			}
			return result_continue;
		}
		case command_info: {printf("step_trigger_time\n"); return result_continue;}

		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_trigger_time_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &trigger, &trigger_parameters, sizeof(short int), sizeof(struct step_trigger_parameters), cmd_data.data_index_trigger, data_size)==1) return result_error;
			
			trigger_parameters->total=(int)((p->maxtime)/cmd_data.time_interval);
			trigger_parameters->current=0;
			trigger_parameters->time=0;
			
			time_elapsed=0;
			
			return result_continue;
		}
		case command_run:
		{
			if (p->time-time_elapsed>cmd_data.time_interval)
			{
				*trigger=1;
				(trigger_parameters->current)++;
				time_elapsed+=cmd_data.time_interval;
				trigger_parameters->time=time_elapsed;

				if (cmd_data.progress==1)
				{
					printf("\b\b\b\b\b");
					printf("%4.1lf%%", 100*(trigger_parameters->current)/(double)(trigger_parameters->total));
					fflush(stdout);
				}
			}
			else *trigger=0;

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_trigger_time_once(short int command, struct step_trigger_time_once_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_trigger_time_once_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	
	static short int already_triggered;

	switch (command)
	{
		case command_info: {printf("step_trigger_time_once\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_trigger_time_once_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &trigger, &trigger_parameters, sizeof(short int), sizeof(struct step_trigger_parameters), cmd_data.data_index_trigger, data_size)==1) return result_error;
			
			trigger_parameters->total=cmd_data.runs;
			trigger_parameters->current=0;
			
			return result_continue;
		}
		case command_begin:
		{
			already_triggered=0;
			
			return result_continue;
		}
		case command_run:
		{
			if (p->time>=cmd_data.time && already_triggered==0)
			{
				*trigger=1;
				(trigger_parameters->current)++;

				already_triggered=1;
			}
			else *trigger=0;

			return result_continue;
		}
		default: return result_continue;
	}
}

