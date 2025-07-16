/**************************************************************
***************************************************************
*** 
***   run.c
***
***   Jason Olejarz
***   2011-01-25
***
***   This file contains the core calculations for evolving the interfaces.
***
***************************************************************
**************************************************************/

#include "includes.h"

short int run (short int (*step[])(), short int step_size, void *data[], void *data_parameters[], short int data_size, struct run_parameters *parameters, short int extra_time_for_blinkers)
{
	//
	// This allows use of bias fields to drive the system into its final state.
	//

	// spin type 11 to spin type 11+(q-2) = 0 next to two 0 and two q_i (q_i=1,2,...,q-1)
	// spin type 11+(q-1) to 11+(q-1)+(q-2) = 1 next to two 1 and two q_i (q_i=2,3,...,q-1,0)
	// etc.

	// Load all parameter values.
	struct rng_struct *rng_struct=parameters->rng_struct;
	short int q=parameters->q;
	int xlen=parameters->xlen;
	int ylen=parameters->ylen;
	short int *spins_initial=parameters->spins_initial;
	short int *spins=parameters->spins;
	short int *state_initial=parameters->state_initial;
	short int *state=parameters->state;
	int *map[11+MAX_Q*(MAX_Q-1)];
	int *index[11+MAX_Q*(MAX_Q-1)];

	short int result, step_index=0, evolve_continue=1;

	// A random number between 0 inclusive and 1 exclusive is stored here.
	double random_number;
	
	int a, b, i, j, k;

	short int q_bias_index;

	int x, y;

	int xminus;
	int xplus;
	int yminus;
	int yplus;

	int area=xlen*ylen;

	short int num_diff;
	short int num_equal;
	short int num_unique_neighbors;

	int new_value_index;

	short int new_value[MAX_Q];

	short int spin_neighbors[MAX_Q];

	int x_neighbor, y_neighbor;

	short int prior_value;

	double energy_before_flip;

	int eligible_remaining;

	// If `update_prob_num' is set to `1', then the sum of probabilities array must be updated.
	short int update_prob_num=1;
	
	for (i=0; i<11+q*(q-1); i++)
	{
		map[i]=parameters->map[i];
		index[i]=parameters->index[i];
	}

	// The probabilities of flipping spins with different energies are stored here.
	double prob[11+MAX_Q*(MAX_Q-1)];
	double prob_num[11+MAX_Q*(MAX_Q-1)];
	int prob_num_type[11+MAX_Q*(MAX_Q-1)];
	
	// The number of spins of different types is stored here.
	int number_of_spin_types;

	// The positions of the four spins neighboring the selected spin (and the four neighbors
	// of one of the neighboring spins) are stored here.
	int position[5];

	// The energies of the selected spins before and after the flip are stored here.
	int type_start[5];
	int type_end[5];

	// The numbers of spins with different energies before the previous step are saved here.
	// These numbers are used to determine if the number of spin types increases or decreases.
	int active[11+MAX_Q*(MAX_Q-1)];
	
	int holes[11+MAX_Q*(MAX_Q-1)][11+MAX_Q*(MAX_Q-1)]={};
	int holes_count[11+MAX_Q*(MAX_Q-1)]={};

	// This is used for updating the time.
	double total_rate;

	if (step!=0)
	{
		memcpy (spins, spins_initial, sizeof(short int)*area);
		memcpy (state, state_initial, sizeof(short int)*area);

		for (i=0; i<11+q*(q-1); i++)
		{
			*(index[i])=0;
		}

		for (j=0; j<area; j++)
		{
			for (i=0; i<11+q*(q-1); i++)
			{
				*(index[i]+1+j)=0;
				*(map[i]+j)=0;
			}
		}

		// Determine the type of each spin.
		for (y=0; y<ylen; y++)
		{
			for (x=0; x<xlen; x++)
			{
				// Determine the coordinates of the neighboring spins, accounting for the boundary conditions.
				if (x==0) xminus=xlen-1;
				else xminus=x-1;
				if (x==xlen-1) xplus=0;
				else xplus=x+1;
				if (y==0) yminus=ylen-1;
				else yminus=y-1;
				if (y==ylen-1) yplus=0;
				else yplus=y+1;

				num_diff=0;
				if (*(spins+x+y*xlen)!=*(spins+xminus+y*xlen)) num_diff++;
				if (*(spins+x+y*xlen)!=*(spins+xplus+y*xlen)) num_diff++;
				if (*(spins+x+y*xlen)!=*(spins+x+yminus*xlen)) num_diff++;
				if (*(spins+x+y*xlen)!=*(spins+x+yplus*xlen)) num_diff++;

				num_unique_neighbors=0;
				for (i=0; i<q; i++)
				{
					spin_neighbors[i]=0;
					if (*(spins+xminus+y*xlen)==i) spin_neighbors[i]++;
					if (*(spins+xplus+y*xlen)==i) spin_neighbors[i]++;
					if (*(spins+x+yminus*xlen)==i) spin_neighbors[i]++;
					if (*(spins+x+yplus*xlen)==i) spin_neighbors[i]++;

					if (spin_neighbors[i]>0) num_unique_neighbors++;
				}

				// Save the type of this spin (between 0 and 11+q*(q-1)-1).
				if (num_diff==0) *(state+x+y*xlen)=0;
				else if (num_diff==1) *(state+x+y*xlen)=1;
				else if (num_diff==2)
				{
					if (num_unique_neighbors==2)
					{
						if (*(spins+x+y*xlen)!=*(spins+xminus+y*xlen))
						{
							*(state+x+y*xlen)=11+(*(spins+x+y*xlen))*(q-1)+((*(spins+xminus+y*xlen))-1-(*(spins+x+y*xlen))+q)%q;
						}
						else if (*(spins+x+y*xlen)!=*(spins+xplus+y*xlen))
						{
							*(state+x+y*xlen)=11+(*(spins+x+y*xlen))*(q-1)+((*(spins+xplus+y*xlen))-1-(*(spins+x+y*xlen))+q)%q;
						}
						else if (*(spins+x+y*xlen)!=*(spins+x+yminus*xlen))
						{
							*(state+x+y*xlen)=11+(*(spins+x+y*xlen))*(q-1)+((*(spins+x+yminus*xlen))-1-(*(spins+x+y*xlen))+q)%q;
						}
						else
						{
							*(state+x+y*xlen)=11+(*(spins+x+y*xlen))*(q-1)+((*(spins+x+yplus*xlen))-1-(*(spins+x+y*xlen))+q)%q;
						}
					}
					else *(state+x+y*xlen)=2;
				}
				else if (num_diff==3)
				{
					if (num_unique_neighbors==2) *(state+x+y*xlen)=3;
					else if (num_unique_neighbors==3) *(state+x+y*xlen)=4;
					else *(state+x+y*xlen)=5;
				}
				else
				{
					if (num_unique_neighbors==1) *(state+x+y*xlen)=6;
					else if (num_unique_neighbors==2)
					{
						for (i=0; i<q; i++)
						{
							if (spin_neighbors[i]==3) break;
						}
						if (i<q) *(state+x+y*xlen)=7;
						else *(state+x+y*xlen)=8;
					}
					else if (num_unique_neighbors==3) *(state+x+y*xlen)=9;
					else *(state+x+y*xlen)=10;
				}

				// Add this spin to the correct list.
				(*index[*(state+x+y*xlen)])++;
				*(index[*(state+x+y*xlen)]+*index[*(state+x+y*xlen)])=x+y*xlen;
				*(map[*(state+x+y*xlen)]+x+y*xlen)=*index[*(state+x+y*xlen)];
			}
		}

		// Determine the energy and magnetization of the system.
		parameters->mag=0;
		for (i=0; i<q; i++)
		{
			parameters->num_value[i]=0;
		}
		parameters->energy=0;
		for (i=0; i<area; i++)
		{
			y=(int)(i/xlen);
			x=(int)(i-y*xlen);

			if (x==xlen-1) position[0]=y*xlen;
			else position[0]=x+1+y*xlen;
			if (y==ylen-1) position[1]=x;
			else position[1]=x+(y+1)*xlen;

			for (j=0; j<2; j++)
			{
				if (*(spins+i)==*(spins+position[j])) parameters->energy--;
			}
			
			parameters->mag+=*(spins+i);

			(parameters->num_value[*(spins+i)])++;
		}
		// This ensures that, with no bias field, the energy is between 0 (ground state) and
		// 2*(number of spins).
		parameters->energy+=2*area;

		parameters->steps=0;
		parameters->time=0;
		
		parameters->coordinate=0;
		parameters->move=0;
		parameters->move_backward=0;
	}

	// Send 'begin' to all functions in 'step'.
	if (step!=0)
	{
		while (step[step_index]!=0 && step_index<step_size)
		{
			if (step[step_index++](command_begin, 0, data, data_parameters, data_size, parameters)==result_error) return result_error;
		}
		step_index=0;
	}

	// If both 'maxsteps' and 'maxtime' are zero, then there is nothing to do.
	if (parameters->maxsteps==0 && parameters->maxtime==0) return 0;
	
	number_of_spin_types=0;
	for (i=0; i<11+q*(q-1); i++)
	{
		active[i]=*index[i];
		if (*index[i]!=0)
		{
			number_of_spin_types++;
		}

		// Set the probability arrays.
		// This is used for correctly computing the time increment on the first step.
		prob[i]=parameters->prob[i];
	}

	parameters->num_energy_drops=0;

	do
	{
		// Increment the number of steps.
		(parameters->steps)++;

		// Update the time elapsed.  The time increment is drawn from an
		// exponential distribution.
		total_rate=0;
		for (i=0; i<11+q*(q-1); i++)
		{
			total_rate+=active[i]*prob[i];
		}
		rng_struct->rng_next(rng_struct);
		random_number=(rng_struct->current_double);
		(parameters->time)+=(double)(-log(1-random_number)/total_rate);

		// Check if the sum of probabilities array must be updated.
		if (update_prob_num==1)
		{
			j=0;
			for (i=0; i<11+q*(q-1); i++)
			{
				if (*index[i]!=0)
				{
					if (j==0) prob_num[j]=(*index[i])*prob[i];
					else prob_num[j]=prob_num[j-1]+(*index[i])*prob[i];
					
					prob_num_type[j]=i;
					j++;
				}
								
				// Check if all spin types in the system are included in the `prob_num' array.
				if (j==number_of_spin_types) break;
			}
			
			// Rescale the `prob_num' array so that any random number between 0 and 1 points
			// to a spin type.
			for (i=0; i<j-1; i++)
			{
				prob_num[i]=prob_num[i]/prob_num[j-1];
			}
			prob_num[j-1]=1;
			
			update_prob_num=0;
		}

		// Generate a random number.
		rng_struct->rng_next(rng_struct);
		random_number=(rng_struct->current_double);

		// Determine the energy of the spin that is selected to flip.
		for (j=0; j<number_of_spin_types; j++)
		{
			if (prob_num[j]>random_number) break;
		}
		type_start[0]=prob_num_type[j];

		// Generate a random number.
		rng_struct->rng_next(rng_struct);
		random_number=(rng_struct->current_double);

		// Choose a spin to flip.	
		j=1+(int)((*index[type_start[0]])*random_number);
		position[0]=*(index[type_start[0]]+j);

		// This is used for updating the magnetization.
		prior_value=*(spins+position[0]);

		// This is used to check if the spin flip lowers the energy.
		energy_before_flip=parameters->energy;

		// Get the x and y coordinates of the selected spin.
		y=(int)(position[0]/xlen);
		x=(int)(position[0]-y*xlen);

		// Determine the coordinates of the neighboring spins, accounting for the boundary conditions.
		if (x==0) position[1]=xlen-1+y*xlen;
		else position[1]=x-1+y*xlen;
		if (x==xlen-1) position[2]=y*xlen;
		else position[2]=x+1+y*xlen;
		if (y==0) position[3]=x+(ylen-1)*xlen;
		else position[3]=x+(y-1)*xlen;
		if (y==ylen-1) position[4]=x;
		else position[4]=x+(y+1)*xlen;

		// Determine the values of the neighboring spins.
		num_unique_neighbors=0;
		for (i=0; i<q; i++)
		{
			spin_neighbors[i]=0;
			if (*(spins+position[1])==i) spin_neighbors[i]++;
			if (*(spins+position[2])==i) spin_neighbors[i]++;
			if (*(spins+position[3])==i) spin_neighbors[i]++;
			if (*(spins+position[4])==i) spin_neighbors[i]++;

			if (spin_neighbors[i]>0) num_unique_neighbors++;
		}

		// Record the backward flip.
		parameters->move_backward=*(spins+position[0]);

		// Generate a random number.
		rng_struct->rng_next(rng_struct);
		random_number=(rng_struct->current_double);

		// Determine the new value of the selected spin.
		if (type_start[0]==0)
		{
			// A spin of this type will never flip at T=0.
		}
		else if (type_start[0]==1)
		{
			// A spin of this type will never flip at T=0.
		}
		else if (type_start[0]==2)
		{
			// A spin of this type will never flip at T=0.
		}
		else if (type_start[0]==3)
		{
			new_value_index=0;

			for (i=1; i<q; i++)
			{
				if (spin_neighbors[((*(spins+position[0]))+i)%q]==3) new_value[new_value_index++]=((*(spins+position[0]))+i)%q;
			}

			*(spins+position[0])=new_value[0];
		}
		else if (type_start[0]==4)
		{
			new_value_index=0;

			for (i=1; i<q; i++)
			{
				if (spin_neighbors[((*(spins+position[0]))+i)%q]==2) new_value[new_value_index++]=((*(spins+position[0]))+i)%q;
			}

			for (i=1; i<q; i++)
			{
				if (spin_neighbors[((*(spins+position[0]))+i)%q]==1) new_value[new_value_index++]=((*(spins+position[0]))+i)%q;
			}

			if (random_number<2/(double)3) *(spins+position[0])=new_value[0];
			else *(spins+position[0])=new_value[1];
		}
		else if (type_start[0]==5)
		{
			new_value_index=0;

			for (i=1; i<q; i++)
			{
				if (spin_neighbors[((*(spins+position[0]))+i)%q]==1) new_value[new_value_index++]=((*(spins+position[0]))+i)%q;
			}

			if (random_number<1/(double)3) *(spins+position[0])=new_value[0];
			else if (random_number<2/(double)3) *(spins+position[0])=new_value[1];
			else *(spins+position[0])=new_value[2];
		}
		else if (type_start[0]==6)
		{
			new_value_index=0;

			for (i=1; i<q; i++)
			{
				if (spin_neighbors[((*(spins+position[0]))+i)%q]==4) new_value[new_value_index++]=((*(spins+position[0]))+i)%q;
			}

			for (i=1; i<q; i++)
			{
				if (spin_neighbors[((*(spins+position[0]))+i)%q]==0) new_value[new_value_index++]=((*(spins+position[0]))+i)%q;
			}

			if (q==2 || random_number<2/(double)q) *(spins+position[0])=new_value[0];
			else
			{
				for (k=1; k<q-2; k++)
				{
					if (random_number<2/(double)q+k*(1-2/(double)q)/(double)(q-2)) {*(spins+position[0])=new_value[k]; break;}
				}
				if (k==q-2) *(spins+position[0])=new_value[k];
			}
		}
		else if (type_start[0]==7)
		{
			new_value_index=0;

			for (i=1; i<q; i++)
			{
				if (spin_neighbors[((*(spins+position[0]))+i)%q]==3) new_value[new_value_index++]=((*(spins+position[0]))+i)%q;
			}

			for (i=1; i<q; i++)
			{
				if (spin_neighbors[((*(spins+position[0]))+i)%q]==1) new_value[new_value_index++]=((*(spins+position[0]))+i)%q;
			}

			for (i=1; i<q; i++)
			{
				if (spin_neighbors[((*(spins+position[0]))+i)%q]==0) new_value[new_value_index++]=((*(spins+position[0]))+i)%q;
			}

			if (random_number<2/(double)(1+q)) *(spins+position[0])=new_value[0];
			else if (q==3 || random_number<4/(double)(1+q)) *(spins+position[0])=new_value[1];
			else
			{
				for (k=1; k<q-3; k++)
				{
					if (random_number<4/(double)(1+q)+k*(1-4/(double)(1+q))/(double)(q-3)) {*(spins+position[0])=new_value[1+k]; break;}
				}
				if (k==q-3) *(spins+position[0])=new_value[1+k];
			}
		}
		else if (type_start[0]==8)
		{
			new_value_index=0;

			for (i=1; i<q; i++)
			{
				if (spin_neighbors[((*(spins+position[0]))+i)%q]==2) new_value[new_value_index++]=((*(spins+position[0]))+i)%q;
			}

			for (i=1; i<q; i++)
			{
				if (spin_neighbors[((*(spins+position[0]))+i)%q]==0) new_value[new_value_index++]=((*(spins+position[0]))+i)%q;
			}

			if (random_number<2/(double)(1+q)) *(spins+position[0])=new_value[0];
			else if (q==3 || random_number<4/(double)(1+q)) *(spins+position[0])=new_value[1];
			else
			{
				for (k=1; k<q-3; k++)
				{
					if (random_number<4/(double)(1+q)+k*(1-4/(double)(1+q))/(double)(q-3)) {*(spins+position[0])=new_value[1+k]; break;}
				}
				if (k==q-3) *(spins+position[0])=new_value[1+k];
			}
		}
		else if (type_start[0]==9)
		{
			new_value_index=0;

			for (i=1; i<q; i++)
			{
				if (spin_neighbors[((*(spins+position[0]))+i)%q]==2) new_value[new_value_index++]=((*(spins+position[0]))+i)%q;
			}

			for (i=1; i<q; i++)
			{
				if (spin_neighbors[((*(spins+position[0]))+i)%q]==1) new_value[new_value_index++]=((*(spins+position[0]))+i)%q;
			}

			for (i=1; i<q; i++)
			{
				if (spin_neighbors[((*(spins+position[0]))+i)%q]==0) new_value[new_value_index++]=((*(spins+position[0]))+i)%q;
			}

			if (random_number<2/(double)(2+q)) *(spins+position[0])=new_value[0];
			else if (random_number<4/(double)(2+q)) *(spins+position[0])=new_value[1];
			else if (q==4 || random_number<6/(double)(2+q)) *(spins+position[0])=new_value[2];
			else
			{
				for (k=1; k<q-4; k++)
				{
					if (random_number<6/(double)(2+q)+k*(1-6/(double)(2+q))/(double)(q-4)) {*(spins+position[0])=new_value[2+k]; break;}
				}
				if (k==q-4) *(spins+position[0])=new_value[2+k];
			}
		}
		else if (type_start[0]==10)
		{
			new_value_index=0;

			for (i=1; i<q; i++)
			{
				if (spin_neighbors[((*(spins+position[0]))+i)%q]==1) new_value[new_value_index++]=((*(spins+position[0]))+i)%q;
			}

			for (i=1; i<q; i++)
			{
				if (spin_neighbors[((*(spins+position[0]))+i)%q]==0) new_value[new_value_index++]=((*(spins+position[0]))+i)%q;
			}

			if (random_number<2/(double)(3+q)) *(spins+position[0])=new_value[0];
			else if (random_number<4/(double)(3+q)) *(spins+position[0])=new_value[1];
			else if (random_number<6/(double)(3+q)) *(spins+position[0])=new_value[2];
			else if (q==5 || random_number<8/(double)(3+q)) *(spins+position[0])=new_value[3];
			else
			{
				for (k=1; k<q-5; k++)
				{
					if (random_number<8/(double)(3+q)+k*(1-8/(double)(3+q))/(double)(q-5)) {*(spins+position[0])=new_value[3+k]; break;}
				}
				if (k==q-5) *(spins+position[0])=new_value[3+k];
			}
		}
		else
		{
			new_value_index=0;

			for (i=1; i<q; i++)
			{
				if (spin_neighbors[((*(spins+position[0]))+i)%q]==2) new_value[new_value_index++]=((*(spins+position[0]))+i)%q;
			}

			*(spins+position[0])=new_value[0];
		}

		// Record the flip.
		parameters->coordinate=position[0];
		parameters->move=*(spins+position[0]);

		// Save the starting and ending energies of the four neighboring spins.
		for (i=0; i<5; i++)
		{
			type_start[i]=*(state+position[i]);

			// Get the x and y coordinates of the selected spin.
			y=(int)(position[i]/xlen);
			x=(int)(position[i]-y*xlen);

			// Determine the coordinates of the neighboring spins, accounting for the boundary conditions.
			if (x==0) xminus=xlen-1;
			else xminus=x-1;
			if (x==xlen-1) xplus=0;
			else xplus=x+1;
			if (y==0) yminus=ylen-1;
			else yminus=y-1;
			if (y==ylen-1) yplus=0;
			else yplus=y+1;

			num_diff=0;
			if (*(spins+x+y*xlen)!=*(spins+xminus+y*xlen)) num_diff++;
			if (*(spins+x+y*xlen)!=*(spins+xplus+y*xlen)) num_diff++;
			if (*(spins+x+y*xlen)!=*(spins+x+yminus*xlen)) num_diff++;
			if (*(spins+x+y*xlen)!=*(spins+x+yplus*xlen)) num_diff++;

			num_unique_neighbors=0;
			for (j=0; j<q; j++)
			{
				spin_neighbors[j]=0;
				if (*(spins+xminus+y*xlen)==j) spin_neighbors[j]++;
				if (*(spins+xplus+y*xlen)==j) spin_neighbors[j]++;
				if (*(spins+x+yminus*xlen)==j) spin_neighbors[j]++;
				if (*(spins+x+yplus*xlen)==j) spin_neighbors[j]++;

				if (spin_neighbors[j]>0) num_unique_neighbors++;
			}

			// Save the type of this spin (between 0 and 11+q*(q-1)-1).
			if (num_diff==0) type_end[i]=0;
			else if (num_diff==1) type_end[i]=1;
			else if (num_diff==2)
			{
				if (num_unique_neighbors==2)
				{
					if (*(spins+x+y*xlen)!=*(spins+xminus+y*xlen))
					{
						type_end[i]=11+(*(spins+x+y*xlen))*(q-1)+((*(spins+xminus+y*xlen))-1-(*(spins+x+y*xlen))+q)%q;
					}
					else if (*(spins+x+y*xlen)!=*(spins+xplus+y*xlen))
					{
						type_end[i]=11+(*(spins+x+y*xlen))*(q-1)+((*(spins+xplus+y*xlen))-1-(*(spins+x+y*xlen))+q)%q;
					}
					else if (*(spins+x+y*xlen)!=*(spins+x+yminus*xlen))
					{
						type_end[i]=11+(*(spins+x+y*xlen))*(q-1)+((*(spins+x+yminus*xlen))-1-(*(spins+x+y*xlen))+q)%q;
					}
					else
					{
						type_end[i]=11+(*(spins+x+y*xlen))*(q-1)+((*(spins+x+yplus*xlen))-1-(*(spins+x+y*xlen))+q)%q;
					}
				}
				else type_end[i]=2;
			}
			else if (num_diff==3)
			{
				if (num_unique_neighbors==2) type_end[i]=3;
				else if (num_unique_neighbors==3) type_end[i]=4;
				else type_end[i]=5;
			}
			else
			{
				if (num_unique_neighbors==1) type_end[i]=6;
				else if (num_unique_neighbors==2)
				{
					for (k=0; k<q; k++)
					{
						if (spin_neighbors[k]==3) break;
					}
					if (k<q) type_end[i]=7;
					else type_end[i]=8;
				}
				else if (num_unique_neighbors==3) type_end[i]=9;
				else type_end[i]=10;
			}
		}

		// Update the map and index arrays for the selected spin and its four neighboring spins.
		for (i=0; i<5; i++)
		{
			*(index[type_start[i]]+*(map[type_start[i]]+position[i]))=0;
			holes[type_start[i]][holes_count[type_start[i]]++]=*(map[type_start[i]]+position[i]);
			*(map[type_start[i]]+position[i])=0;

			(*index[type_end[i]])++;
			*(index[type_end[i]]+*index[type_end[i]])=position[i];
			*(map[type_end[i]]+position[i])=*index[type_end[i]];

			*(state+position[i])=type_end[i];
		}

		// Fill in holes in the active spins arrays.
		for (i=0; i<11+q*(q-1); i++)
		{
			holes_count[i]--;
			while (holes_count[i]>=0)
			{
				if (*(index[i]+*index[i])!=0 || (*(index[i]+*index[i])==0 && *map[i]==*index[i]))
				{
					*(index[i]+holes[i][holes_count[i]])=*(index[i]+*index[i]);
					*(map[i]+*(index[i]+*index[i]))=holes[i][holes_count[i]];
					*(index[i]+*index[i])=0;
					holes[i][holes_count[i]]=0;
				}
				else
				{
					for (a=0; a<11+q*(q-1)-1; a++)
					{
						if (holes[i][a]==*index[i])
						{
							for (b=a; b<11+q*(q-1)-1; b++)
							{
								holes[i][b]=holes[i][b+1];
							}
							break;
						}
					}
				}
				
				(*index[i])--;
				holes_count[i]--;

				if (*index[i]==0) break;
			}
			holes_count[i]=0;
		}

		// Check if the sum of probabilities array needs updating.
		for (i=0; i<11+q*(q-1); i++)
		{
			if (active[i]!=*index[i])
			{
				update_prob_num=1;
				
				if (active[i]!=0 && *index[i]==0) number_of_spin_types--;
				if (active[i]==0 && *index[i]!=0) number_of_spin_types++;
			}
		}
		
		for (i=0; i<11+q*(q-1); i++)
		{
			active[i]=*index[i];
		}

		// Update the energy.
		if (type_start[0]==3) parameters->energy-=2;
		else if (type_start[0]==4)
		{
			if (type_end[0]==2) parameters->energy--;
		}
		else if (type_start[0]==6)
		{
			if (type_end[0]==0) parameters->energy-=4;
		}
		else if (type_start[0]==7)
		{
			if (type_end[0]==1) parameters->energy-=3;
			else if (type_end[0]==3) parameters->energy--;
		}
		else if (type_start[0]==8)
		{
			if (type_end[0]>10) parameters->energy-=2;
		}
		else if (type_start[0]==9)
		{
			if (type_end[0]==2) parameters->energy-=2;
			else if (type_end[0]==4) parameters->energy--;
		}
		else if (type_start[0]==10)
		{
			if (type_end[0]==5) parameters->energy--;
		}

		// This is used by the forcing function to keep track of the number of energy drops
		// as the bias fields are imposed.
		if (energy_before_flip>parameters->energy) (parameters->num_energy_drops)++;
		
		// Update the magnetization.
		parameters->mag+=*(spins+position[0])-prior_value;
		
		// Update the numbers of each spin.
		for (k=0; k<q; k++)
		{
			if (prior_value==k) {(parameters->num_value[k])--; break;}
		}
		for (k=0; k<q; k++)
		{
			if (*(spins+position[0])==k) {(parameters->num_value[k])++; break;}
		}

		// Do intermediate processing.
		if (step!=0)
		{
			while (step[step_index]!=0 && step_index<step_size)
			{
				result=step[step_index++](command_run, 0, data, data_parameters, data_size, parameters);
				if (result==result_error) return result_error;
				else if (result==result_done) {evolve_continue=0; break;}
			}
			step_index=0;
		}

		// If the system is being driven with a bias field, determine when to stop the simulation.
		if (step==0)
		{
			if (extra_time_for_blinkers==0)
			{
				if (*index[3]==0 && *index[4]==0 && *index[6]==0 && *index[7]==0 && *index[8]==0 && *index[9]==0 && *index[10]==0)
				{
					eligible_remaining=0;
					for (i=0; i<q*(q-1); i++)
					{
						if (*index[11+i]==0 && prob[11+i]>0) eligible_remaining++;
					}

					if (eligible_remaining==q-1) evolve_continue=0;
				}
			}
			else if (parameters->time>=parameters->maxtime) evolve_continue=0;
		}

		eligible_remaining=0;
		for (i=3; i<11+q*(q-1); i++)
		{
			if (*index[i]>0) eligible_remaining++;
		}
	}
	while (evolve_continue==1 && (step!=0 &&
		eligible_remaining>0 &&
		((parameters->steps>0 && parameters->steps<parameters->maxsteps) ||
		(parameters->time>0 && parameters->time<parameters->maxtime))
		|| step==0));

	// Send 'end' to all functions in 'step'.
	if (step!=0)
	{
		while (step[step_index]!=0 && step_index<step_size)
		{
			if (step[step_index++](command_end, 0, data, data_parameters, data_size, parameters)==result_error) return result_error;
		}
		step_index=0;
	}

	return 0;
}
