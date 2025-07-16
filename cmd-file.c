/**************************************************************
***************************************************************
*** 
***   cmd-file.c
***
***   Jason Olejarz
***   2011-02-15
***
***   This file contains commands for file I/O.
***
***************************************************************
**************************************************************/

void cmd_load()
{
	short int data_index=arg_short_int[0];

	unsigned long int size;
	
	void *output;
	int *output_size;

	FILE *datafile=fopen(arg_string[0], "r");
	
	fseek (datafile, 0, SEEK_END);
	size = ftell (datafile);
	rewind (datafile);
  
	if (data_output(data, data_parameters, &output, &output_size, size, sizeof(int), data_index, data_num)==1) return;

	size=fread(output, 1, size, datafile);

	fclose(datafile);
	
	*output_size=size;
	
	printf("%d bytes were read from %s.\n", size, arg_string[0]);

	return;
}

void cmd_loadtext()
{
	short int data_index=arg_short_int[0];

	double *output;
	int *output_size;

	double value;
	int number;

	FILE *datafile=fopen(arg_string[0], "r");
	
	number=0;
	while (fscanf(datafile, "%lf", &value)!=EOF) number++;

	rewind (datafile);

	if (data_output(data, data_parameters, &output, &output_size, number*sizeof(double), sizeof(int), data_index, data_num)==1) return;

	number=0;
	while (fscanf(datafile, "%lf", &value)!=EOF) *(output+(number++))=value;

	fclose(datafile);
	
	*output_size=number*sizeof(double);
	
	printf("%d numbers were read from %s.\n", number, arg_string[0]);

	return;
}

void cmd_load2d()
{
	short int data_index=arg_short_int[0];

	unsigned long int size;

	struct run_parameters *parameters;
	int *parameters_size;
	
	if (data_input(data, data_parameters, &parameters, &parameters_size, data_index, data_num)==1) return;

	FILE *datafile=fopen(arg_string[0], "r");
	
	fseek (datafile, 0, SEEK_END);
	size=ftell (datafile);
	rewind (datafile);

	int x, y;
	int xlen=parameters->xlen, ylen=parameters->ylen;
	int current;

	//fread (&((parameters->rng_struct)->current_long_int), 1, sizeof(unsigned long int), datafile);
	//((parameters->rng_struct)->rng_seed)(parameters->rng_struct, ((parameters->rng_struct)->current_long_int));
	//
	// The state of the RNG is not saved by `save2d'.  If the state of the RNG was saved, then
	// the map and index arrays should also be saved.  This would require a lot of disk space.
	//
	fread (&(parameters->steps), 1, sizeof(unsigned long int), datafile);
	fread (&(parameters->time), 1, sizeof(double), datafile);

	for (y=0; y<ylen; y++)
	{
		for (x=0; x<xlen; x++)
		{
			fread (parameters->spins_initial+x+y*xlen, 1, sizeof(short int), datafile);
		}
	}

	fclose(datafile);
	
	return;
}

void cmd_numclust()
{
	short int data_index=arg_short_int[0];
	short int data_index_clusters_index=arg_short_int[1];
	short int spin_type=arg_short_int[2];
	int num_runs=arg_int[0];
	double x_value=arg_double[0];

	double *data_list_double;
	int *data_list_parameters;

	double *clusters_index;
	int *clusters_index_parameters;

	int data_number, i, j;

	int clusters;

	double average, stdev, sum, sum_sq;

	if (data_input(data, data_parameters, &data_list_double, &data_list_parameters, data_index, data_num)==1) return;
	data_number=*data_list_parameters/sizeof(double);

	if (data_input(data, data_parameters, &clusters_index, &clusters_index_parameters, data_index_clusters_index, data_num)==1) return;

	sum=0;
	sum_sq=0;

	j=0;
	for (i=0; i<data_number; i++)
	{
		if (*(clusters_index+j)==i)
		{
			if (i>0)
			{
				sum+=clusters;
				sum_sq+=clusters*clusters;
			}

			clusters=0;

			j++;
		}

		if (*(data_list_double+i)==spin_type)
		{
			clusters++;
		}
	}
	sum+=clusters;
	sum_sq+=clusters*clusters;

	average=sum/(double)num_runs;
	stdev=sqrt(sum_sq/(double)num_runs-pow(average,2));

	FILE *datafile=fopen(arg_string[0], "a");

	fprintf (datafile, "%.10lf %.10lf %.10lf\n", x_value, average, stdev/sqrt(num_runs));

	fclose(datafile);

	return;
}

void cmd_save()
{
	short int data_index=arg_short_int[0];

	void *input;
	int *input_size;
	
	if (data_input(data, data_parameters, &input, &input_size, data_index, data_num)==1) return;

	FILE *datafile=fopen(arg_string[0], "w");
		
	fwrite(input, 1, *input_size, datafile);
	
	fclose(datafile);
	
	printf("%d bytes were written to %s.\n", *input_size, arg_string[0]);
	
	return;
}

void cmd_savetext()
{
	// All data in the specified data block is written in text format to the output file.
	
	short int data_index=arg_short_int[0];

	double *input;
	int *input_size;
	
	if (data_input(data, data_parameters, &input, &input_size, data_index, data_num)==1) return;

	FILE *datafile=fopen(arg_string[0], "w");
	
	int i;
	
	for (i=0; i<(*input_size)/sizeof(double); i++)
	{
		fprintf(datafile, "%.10lf\n", *(input+i));
	}
	
	fclose(datafile);
	
	printf("%d numbers were written to %s.\n", (*input_size)/sizeof(double), arg_string[0]);
	
	return;
}

void cmd_save2d()
{
	short int data_index=arg_short_int[0];

	struct run_parameters *parameters;
	int *parameters_size;

	if (data_input(data, data_parameters, &parameters, &parameters_size, data_index, data_num)==1) return;

	FILE *datafile=fopen(arg_string[0], "w");

	//fwrite(&((parameters->rng_struct)->current_long_int), 1, sizeof(unsigned long int), datafile);
	//
	// The state of the RNG is not saved by `save3d'.  If the state of the RNG was saved, then
	// the map and index arrays should also be saved.  This would require a lot of disk space.
	//
	fwrite(&(parameters->steps), 1, sizeof(unsigned long int), datafile);
	fwrite(&(parameters->time), 1, sizeof(double), datafile);

	int x, y;
	int xlen=parameters->xlen, ylen=parameters->ylen;

	for (y=0; y<ylen; y++)
	{
		for (x=0; x<xlen; x++)
		{
			fwrite (parameters->spins+x+y*xlen, 1, sizeof(short int), datafile);
		}
	}

	fclose(datafile);

	return;
}

void cmd_sizefile()
{
	short int data_index=arg_short_int[0];
	int num_runs=arg_int[0];
	double x_coordinate_value=arg_double[0];
	
	double *data_list_double;
	
	int *data_list_parameters;
	
	int data_number;

	double prob;
	
	if (data_input(data, data_parameters, &data_list_double, &data_list_parameters, data_index, data_num)==1) return;
	data_number=*data_list_parameters/sizeof(double);

	prob=data_number/(double)num_runs;
	
	FILE *datafile=fopen(arg_string[0], "w");

	fprintf(datafile, "%.10lf %.10lf %.10lf\n", x_coordinate_value, prob, sqrt(prob*(1-prob)/(double)num_runs));

	fclose(datafile);

	return;
}

void cmd_xy()
{
	short int data_index_x=arg_short_int[0];
	short int data_index_y=arg_short_int[1];

	if (data[data_index_x]==0)
	{
		printf("The x array is empty.\n");
		return;
	}
	if (data[data_index_y]==0)
	{
		printf("The y array is empty.\n");
		return;
	}

	double *x_data=data[data_index_x], *y_data=data[data_index_y];
	
	int *x_size=data_parameters[data_index_x], *y_size=data_parameters[data_index_y];
	if (*x_size!=*y_size)
	{
		printf("The sizes of the x and y arrays must be the same.\n");
		return;
	}
	
	int i;
	
	FILE *data=fopen(arg_string[0], "w");

	for (i=0; i<(*x_size/sizeof(double)); i++)
	{
		fprintf (data, "%.10lf %.10lf\n", *(x_data+i), *(y_data+i));
	}
	
	fclose(data);
	
	return;
}

void cmd_xyrange()
{
	short int data_index_x=arg_short_int[0];
	short int data_index_y=arg_short_int[1];
	int starting_position=arg_int[0];
	int ending_position=arg_int[1];

	if (data[data_index_x]==0)
	{
		printf("The x array is empty.\n");
		return;
	}
	if (data[data_index_y]==0)
	{
		printf("The y array is empty.\n");
		return;
	}

	double *x_data=data[data_index_x], *y_data=data[data_index_y];
	
	int *x_size=data_parameters[data_index_x], *y_size=data_parameters[data_index_y];
	if (*x_size!=*y_size)
	{
		printf("The sizes of the x and y arrays must be the same.\n");
		return;
	}
	
	int i;
	
	FILE *data=fopen(arg_string[0], "w");

	for (i=starting_position; i<=ending_position; i++)
	{
		fprintf (data, "%.10lf %.10lf\n", *(x_data+i), *(y_data+i));
	}
	
	fclose(data);
	
	return;
}

void cmd_xyyerror()
{
	short int data_index=arg_short_int[0];
	int bin_size=arg_int[0];
	double x_value=arg_double[0];
	
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
	
	FILE *datafile=fopen(arg_string[0], "a");

	fprintf (datafile, "%.10lf %.10lf %.10lf\n", x_value, average, stdev/sqrt(num_bins));
	
	fclose(datafile);

	free (bin_averages);

	return;
}

