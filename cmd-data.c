/**************************************************************
***************************************************************
*** 
***   cmd-data.c
***
***   Jason Olejarz
***   2011-01-26
***
***   This file contains commands for manipulating data.
***
***************************************************************
**************************************************************/

void cmd_add()
{
	short int data_index_input=arg_short_int[0];
	short int data_index_output=arg_short_int[1];
	double number=arg_double[0];

	double *input, *output;
	int *input_size, *output_size;

	if (data_input(data, data_parameters, &input, &input_size, data_index_input, data_num)==1) return;
	if (data_output(data, data_parameters, &output, &output_size, *input_size, sizeof(int), data_index_output, data_num)==1) return;

	*output_size=*input_size;

	int data_number=*input_size/sizeof(double);

	int i;

	for (i=0; i<data_number; i++)
	{
		*(output+i)=*(input+i)+number;
	}

	return;
}

void cmd_bins()
{
	short int data_index=arg_short_int[0];
	int bin_size=arg_int[0];
	
	double *data_list_double;
	
	int *data_list_parameters;
	
	int data_number, i, j, num_bins;
	
	double *bin_averages;
	
	double average, stdev, sum_sq;
	
	if (data_input(data, data_parameters, &data_list_double, &data_list_parameters, data_index, data_num)==1) return;
	data_number=*data_list_parameters/sizeof(double);
	
	if (data_number%bin_size!=0) {printf ("The data set must be able to be broken into an integer number of bins.\n"); return;}
	num_bins=data_number/bin_size;
	bin_averages=malloc (sizeof(double)*data_number/bin_size);
	
	for (j=0; j<num_bins; j++)
	{
		*(bin_averages+j)=0;
		for (i=0; i<bin_size; i++)
		{
			*(bin_averages+j)+=*(data_list_double+j*bin_size+i);
		}
		*(bin_averages+j)/=(double)bin_size;
	}
	
	average=0; sum_sq=0;
	for (j=0; j<num_bins; j++)
	{
		average+=*(bin_averages+j);
		sum_sq+=pow(*(bin_averages+j),2);
	}
	average/=(double)num_bins;
	stdev=sqrt(sum_sq/(double)num_bins-pow(average,2));
	
	printf ("%lf %lf\n", average, stdev/sqrt(num_bins));
	
	free (bin_averages);
	
	return;
}

void cmd_cnt()
{
        short int data_index=arg_short_int[0];
        double min=arg_double[0];
        double max=arg_double[1];

        double *data_list_double;

        int *data_list_parameters;

        int data_number, i;

        int count=0;

        if (data_input(data, data_parameters, &data_list_double, &data_list_parameters, data_index, data_num)==1) return;
        data_number=*data_list_parameters/sizeof(double);

        for (i=0; i<data_number; i++)
        {
                if (*(data_list_double+i)>=min && *(data_list_double+i)<=max) count++;
        }

	printf("%d\n", count);

	return;
}

void cmd_dd()
{
	data_delete(data, data_parameters, arg_short_int[0], data_num);
	return;
}

void cmd_dindex()
{
/*
	// Delete all entries from the parallel index list
	parallel_index(0, 0, 0, data_num);
*/
	return;
}

void cmd_disp()
{
	short int data_index=arg_short_int[0];
	int min=arg_int[0];
	int max=arg_int[1];
	
	double *data_list_double;
	
	void *data_list_parameters;
	
	if (data_input(data, data_parameters, &data_list_double, &data_list_parameters, data_index, data_num)==1) return;
	
	int i;
	
	for (i=min; i<=max; i++)
	{
		printf("%.10lf\n", *(data_list_double+i));
	}
	
	return;
}

void cmd_dt()
{
	// Delete all entries from the tag list
	tag(0, 0, 0, data_num);
	return;
}

void cmd_flt()
{
        short int data_index=arg_short_int[0];
        double min=arg_double[0];
        double max=arg_double[1];

        double *input[data_num/2], *output[data_num/2];
        int *input_size[data_num/2], *output_size[data_num/2];

        struct tag_data a=tag(2, 0, 0, data_num);

        int i, j, k, output_index=0, count=0;

        for (i=0; i<data_num/2; i++)
        {
                if (a.data_input[i]!=a.data_output[i])
                {
                        if (data_input(data, data_parameters, &input[i], &input_size[i], a.data_input[i], data_num)==1) return;
                        if (data_output(data, data_parameters, &output[i], &output_size[i], *input_size[i], sizeof(int), a.data_output[i], data_num)==1) return;

                        *output_size[i]=*input_size[i];

                        if (a.data_input[i]==data_index) j=i;

                        count++;
                }
                else break;
        }

        for (i=1; i<count; i++)
        {
                if (*input_size[i]!=*input_size[i-1])
                {
                        printf("The sizes of all tagged input arrays must be the same.\n");
                        return;
                }
        }

        int data_number_input=*input_size[j]/sizeof(double);

        for (i=0; i<data_number_input; i++)
        {
                if (*(input[j]+i)>=min && *(input[j]+i)<=max)
                {
                        for (k=0; k<count; k++)
                        {
                                *(output[k]+output_index)=*(input[k]+i);
                        }
                        output_index++;
                }
        }

        int data_number_output=output_index*sizeof(double);

        for (i=0; i<count; i++)
        {
                *output_size[i]=data_number_output;
        }

        printf("%d numbers were written to each of the output data blocks.\n", output_index);

        return;
}

void cmd_index()
{
/*
	short int data_index=arg_short_int[0];
	int data_size=arg_int[0];

	// Add the data block to the index list for transfer in a parallel computation
	parallel_index(1, data_index, data_size, data_num);
*/
	return;
}

void cmd_minmax()
{
        short int data_index=arg_short_int[0];

        double *input;
        int *input_size;
        int data_number;

        if (data_input(data, data_parameters, &input, &input_size, data_index, data_num)==1) return;
        data_number=*input_size/sizeof(double);

        double min;
        double max;

        int i;

        min=*input;
        max=*input;
        for (i=1; i<data_number; i++)
        {
                if (*(input+i)<min) min=*(input+i);
                if (*(input+i)>max) max=*(input+i);
        }

        printf("min=%lf, max=%lf\n", min, max);

        return;
}

void cmd_moveflips()
{
        short int data_index_parameters=arg_short_int[0];
        short int data_index_flips=arg_short_int[1];
	int delta_x=arg_int[0];
	int delta_y=arg_int[1];

	if (delta_x<0 || delta_y<0)
	{
		printf("(delta x) and (delta y) must both be nonnegative.\n");
		return;
	}

	struct run_parameters *p;
	int *p_size;

	int *flips;
	struct step_flips_parameters *flips_parameters;

        if (data_input(data, data_parameters, &p, &p_size, data_index_parameters, data_num)==1) return;
	if (data_input(data, data_parameters, &flips, &flips_parameters, data_index_flips, data_num)==1) return;

	int xlen=p->xlen, ylen=p->ylen;

	unsigned long int i;

        int x, y, x_new, y_new;

	for (i=0; i<(p->steps); i++)
	{
		y=(*(flips+2*i))/xlen;
		x=(*(flips+2*i))%xlen;

		x_new=(x+delta_x)%xlen;
		y_new=(y+delta_y)%ylen;

		*(flips+2*i)=x_new+y_new*xlen;
	}

        return;
}

void cmd_movelat()
{
        short int data_index_parameters=arg_short_int[0];
	int delta_x=arg_int[0];
	int delta_y=arg_int[1];

	if (delta_x<0 || delta_y<0)
	{
		printf("(delta x) and (delta y) must both be nonnegative.\n");
		return;
	}

	struct run_parameters *p;
	int *p_size;

        if (data_input(data, data_parameters, &p, &p_size, data_index_parameters, data_num)==1) return;

	int xlen=p->xlen, ylen=p->ylen;

        int i, j, x, y, x_new, y_new;

	short int *spins_backup;
	short int *state_backup;
	int *map_backup[11+MAX_Q*(MAX_Q-1)];
	int *index_backup[11+MAX_Q*(MAX_Q-1)];
	short int *T_vertices_backup;
	short int *T_vertices_2_backup;

	short int q=p->q;

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

	// Backup all simulation data.
	memcpy (spins_backup, p->spins, sizeof(short int)*(p->xlen)*(p->ylen));
	memcpy (state_backup, p->state, sizeof(short int)*(p->xlen)*(p->ylen));
	for (i=0; i<11+q*(q-1); i++)
	{
		memcpy (map_backup[i], p->map[i], sizeof(int)*(p->xlen)*(p->ylen));
		memcpy (index_backup[i], p->index[i], sizeof(int)*(1+(p->xlen)*(p->ylen)));
	}
	memcpy (T_vertices_backup, p->T_vertices, sizeof(short int)*(p->xlen)*(p->ylen));
	memcpy (T_vertices_2_backup, p->T_vertices_2, sizeof(short int)*(p->xlen)*(p->ylen));

	for (y=0; y<ylen; y++)
	{
		for (x=0; x<xlen; x++)
		{
			x_new=(x+delta_x)%xlen;
			y_new=(y+delta_y)%ylen;

			*(p->spins+x_new+y_new*xlen)=*(spins_backup+x+y*xlen);
			*(p->state+x_new+y_new*xlen)=*(state_backup+x+y*xlen);
			for (i=0; i<11+q*(q-1); i++)
			{
				*(p->map[i]+x_new+y_new*xlen)=*(map_backup[i]+x+y*xlen);
			}
			*(p->T_vertices+x_new+y_new*xlen)=*(T_vertices_backup+x+y*xlen);
			*(p->T_vertices_2+x_new+y_new*xlen)=*(T_vertices_2_backup+x+y*xlen);
		}
	}

	for (i=0; i<xlen*ylen; i++)
	{
		for (j=0; j<11+q*(q-1); j++)
		{
			if (*(p->index[j])>i)
			{
				y=(*(index_backup[j]+1+i))/xlen;
				x=(*(index_backup[j]+1+i))%xlen;

				x_new=(x+delta_x)%xlen;
				y_new=(y+delta_y)%ylen;

				*(p->index[j]+1+i)=x_new+y_new*xlen;
			}
		}
	}

	y=(p->coordinate)/xlen;
	x=(p->coordinate)%xlen;

	x_new=(x+delta_x)%xlen;
	y_new=(y+delta_y)%ylen;

	p->coordinate=x_new+y_new*xlen;

        return;
}

void cmd_receive()
{
/*
	// If this process is the master (rank 0), then receive all data from the slaves.
	
	int size;
	int rank;
	
	int i, j;
	
	// This structure contains info on all the data blocks that are transferred from the slaves.
	struct parallel_index_data index_data=parallel_index(2, 0, 0, data_num);
	
	// This array contains pointers to the data blocks that the transferred data are put into.
	static double *all_data[data_num];
	static struct save_value_parameters *all_data_parameters[data_num];
	
	// Get the number of processes.
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	// Get this process's rank.
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	// If not the master, or if there is only one process, then don't do any processing here.
	if (rank>0 || size==1) return;
	
	// Create the storage space for the transferred data.
	for (i=0; i<index_data.num; i++)
	{
		if (data_output(data, data_parameters, &all_data[i], &all_data_parameters[i], (size-1)*index_data.data_size[i], sizeof(struct save_value_parameters), index_data.data_index[i], data_num)==1) return;
	}
	
	for (j=0; j<index_data.num; j++)
	{
		all_data_parameters[j]->size=0;
	}

	// Get the data from each slave process.
	for (i=1; i<size; i++)
	{
		for (j=0; j<index_data.num; j++)
		{
			MPI_Recv(	data[index_data.data_index[j]]+(i-1)*index_data.data_size[j],
					index_data.data_size[j]/sizeof(double),
					MPI_DOUBLE,
					i,
					0,
					MPI_COMM_WORLD,
					MPI_STATUS_IGNORE
				);
			all_data_parameters[j]->size+=index_data.data_size[j];
		}
		printf("Received all data from process %d.\n", i);
	}
*/
	return;
}

void cmd_sd()
{
	int i;
	
	for (i=0; i<data_num; i++)
	{
		printf("%d  ", i);
		if (data[i]!=0) printf("occupied\n"); else printf("free\n");
	}
	
	return;
}

void cmd_send()
{
/*
	// If this process is a slave (rank>0), then send its data to the master.
	
	int rank;
	
	int i;
	
	// This structure contains info on all the data blocks that are transferred from the slaves.
	struct parallel_index_data index_data=parallel_index(2, 0, 0, data_num);

	// Get this process's rank.
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	// If not a slave, then don't do any processing here.
	if (rank==0) return;
	
	// Send all data to the master.
	for (i=0; i<index_data.num; i++)
	{
		MPI_Ssend(	data[index_data.data_index[i]],
				index_data.data_size[i]/sizeof(double),
				MPI_DOUBLE,
				0,
				0,
				MPI_COMM_WORLD
			);
	}
	
	// Exit this process.
	MPI_Finalize();
	exit(0);
*/
	return;
}

void cmd_tag()
{
        short int data_index_input=arg_short_int[0];
        short int data_index_output=arg_short_int[1];
    
        // Add the data block to the tag list.
        tag(1, data_index_input, data_index_output, data_num);
    
        return;
}

void cmd_testinit()
{
        short int data_index_parameters=arg_short_int[0];
    
        struct run_parameters *parameters;
        int *parameters_size;

	int i;

	short int value_count[MAX_Q];

        if (data_input(data, data_parameters, &parameters, &parameters_size, data_index_parameters, data_num)==1) return;

	for (i=0; i<(parameters->q); i++)
	{
		value_count[i]=0;
	}

	for (i=0; i<(parameters->xlen)*(parameters->ylen); i++)
	{
		value_count[*(parameters->spins_initial+i)]++;
	}

	for (i=0; i<(parameters->q); i++)
	{
		printf("value=%d, num=%d\n", i, value_count[i]);
	}

        return;
}
