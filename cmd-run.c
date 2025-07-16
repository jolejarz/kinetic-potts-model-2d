/**************************************************************
***************************************************************
*** 
***   cmd-run.c
***
***   Jason Olejarz
***   2011-01-25
***
***   This file contains the command for running the simulations.
***
***************************************************************
**************************************************************/

void cmd_run()
{
	// If this is the master process, do not do any calculation.
	int rank;
	int size;
//	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//	MPI_Comm_size(MPI_COMM_WORLD, &size);
//	if (rank==0 && size>1) return;
	
	short int parameters_data_index=arg_short_int[0];

	int i;

	struct run_parameters *parameters;
	int *parameters_size;

	if (data_input(data, data_parameters, &parameters, &parameters_size, parameters_data_index, data_num)==1) return result_error;

	int q=parameters->q;

	parameters->prob[0]=0;
	parameters->prob[1]=0;
	parameters->prob[2]=0;
	parameters->prob[3]=1;
	parameters->prob[4]=1.5;
	parameters->prob[5]=1.5;
	parameters->prob[6]=q/(double)2;
	parameters->prob[7]=(1+q)/(double)2;
	parameters->prob[8]=(1+q)/(double)2;
	parameters->prob[9]=(2+q)/(double)2;
	parameters->prob[10]=(3+q)/(double)2;
	for (i=0; i<q*(q-1); i++)
	{
		parameters->prob[11+i]=0.5;
	}

	calc(loop, step, save, loop_num, step_num, save_num, data_num, data, data_parameters, run, parameters, 0);

	return;
}
