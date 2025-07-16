/**************************************************************
***************************************************************
*** 
***   cmd-loop.c
***
***   Jason Olejarz
***   2011-01-25
***
***   This file contains commands for setting up preprocessing.
***
***************************************************************
**************************************************************/

void cmd_lp()
{
	short int loop_index=arg_short_int[0];
	int count=arg_int[0];
	short int progress=arg_short_int[1];
	
	struct loop_iterate_data command_data;
	
	command_data.count=count;
	command_data.progress=progress;
	loop_iterate(command_insert, &command_data, data, data_parameters, data_num);

	list_insert(loop, loop_index, loop_iterate, loop_num);
	
	return;
}

void cmd_lpseed()
{
	short int loop_index=arg_short_int[0];
	short int data_index=arg_short_int[1];
	short int data_index_low=arg_short_int[2];
	short int data_index_high=arg_short_int[3];
	int num=arg_int[0];
	
	struct loop_seed_data command_data;
	
	command_data.rng_struct=data[data_index];
	command_data.data_index_low=data_index_low;
	command_data.data_index_high=data_index_high;
	command_data.num=num;
	loop_seed(command_insert, &command_data, data, data_parameters, data_num);

	list_insert(loop, loop_index, loop_seed, loop_num);
	
	return;
}

void cmd_randdiff()
{
	short int loop_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	
	struct loop_randdiff_data randdiffdata;
	
	randdiffdata.data_index_parameters=data_index_parameters;

	loop_rand_diff(command_insert, &randdiffdata, data, data_parameters, data_num);

	list_insert(loop, loop_index, loop_rand_diff, loop_num);
	
	return;
}

void cmd_randq()
{
	short int loop_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	
	struct loop_randq_data randqdata;
	
	randqdata.data_index_parameters=data_index_parameters;

	loop_randq_equal(command_insert, &randqdata, data, data_parameters, data_num);

	list_insert(loop, loop_index, loop_randq_equal, loop_num);
	
	return;
}

void cmd_randq3()
{
	short int loop_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	int rho_numerator_0=arg_int[0];
	int rho_denominator_0=arg_int[1];
	int rho_numerator_1=arg_int[2];
	int rho_denominator_1=arg_int[3];
	
	struct loop_randq_data randqdata;
	
	randqdata.data_index_parameters=data_index_parameters;
	randqdata.rho_numerator[0]=rho_numerator_0;
	randqdata.rho_denominator[0]=rho_denominator_0;
	randqdata.rho_numerator[1]=rho_numerator_1;
	randqdata.rho_denominator[1]=rho_denominator_1;

	loop_randq(command_insert, &randqdata, data, data_parameters, data_num);

	list_insert(loop, loop_index, loop_randq, loop_num);
	
	return;
}

void cmd_randq4()
{
	short int loop_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	int rho_numerator_0=arg_int[0];
	int rho_denominator_0=arg_int[1];
	int rho_numerator_1=arg_int[2];
	int rho_denominator_1=arg_int[3];
	int rho_numerator_2=arg_int[4];
	int rho_denominator_2=arg_int[5];
	
	struct loop_randq_data randqdata;
	
	randqdata.data_index_parameters=data_index_parameters;
	randqdata.rho_numerator[0]=rho_numerator_0;
	randqdata.rho_denominator[0]=rho_denominator_0;
	randqdata.rho_numerator[1]=rho_numerator_1;
	randqdata.rho_denominator[1]=rho_denominator_1;
	randqdata.rho_numerator[2]=rho_numerator_2;
	randqdata.rho_denominator[2]=rho_denominator_2;

	loop_randq(command_insert, &randqdata, data, data_parameters, data_num);

	list_insert(loop, loop_index, loop_randq, loop_num);
	
	return;
}

void cmd_randq5()
{
	short int loop_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	int rho_numerator_0=arg_int[0];
	int rho_denominator_0=arg_int[1];
	int rho_numerator_1=arg_int[2];
	int rho_denominator_1=arg_int[3];
	int rho_numerator_2=arg_int[4];
	int rho_denominator_2=arg_int[5];
	int rho_numerator_3=arg_int[6];
	int rho_denominator_3=arg_int[7];
	
	struct loop_randq_data randqdata;
	
	randqdata.data_index_parameters=data_index_parameters;
	randqdata.rho_numerator[0]=rho_numerator_0;
	randqdata.rho_denominator[0]=rho_denominator_0;
	randqdata.rho_numerator[1]=rho_numerator_1;
	randqdata.rho_denominator[1]=rho_denominator_1;
	randqdata.rho_numerator[2]=rho_numerator_2;
	randqdata.rho_denominator[2]=rho_denominator_2;
	randqdata.rho_numerator[3]=rho_numerator_3;
	randqdata.rho_denominator[3]=rho_denominator_3;

	loop_randq(command_insert, &randqdata, data, data_parameters, data_num);

	list_insert(loop, loop_index, loop_randq, loop_num);
	
	return;
}

void cmd_randq6()
{
	short int loop_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	int rho_numerator_0=arg_int[0];
	int rho_denominator_0=arg_int[1];
	int rho_numerator_1=arg_int[2];
	int rho_denominator_1=arg_int[3];
	int rho_numerator_2=arg_int[4];
	int rho_denominator_2=arg_int[5];
	int rho_numerator_3=arg_int[6];
	int rho_denominator_3=arg_int[7];
	int rho_numerator_4=arg_int[8];
	int rho_denominator_4=arg_int[9];
	
	struct loop_randq_data randqdata;
	
	randqdata.data_index_parameters=data_index_parameters;
	randqdata.rho_numerator[0]=rho_numerator_0;
	randqdata.rho_denominator[0]=rho_denominator_0;
	randqdata.rho_numerator[1]=rho_numerator_1;
	randqdata.rho_denominator[1]=rho_denominator_1;
	randqdata.rho_numerator[2]=rho_numerator_2;
	randqdata.rho_denominator[2]=rho_denominator_2;
	randqdata.rho_numerator[3]=rho_numerator_3;
	randqdata.rho_denominator[3]=rho_denominator_3;
	randqdata.rho_numerator[4]=rho_numerator_4;
	randqdata.rho_denominator[4]=rho_denominator_4;

	loop_randq(command_insert, &randqdata, data, data_parameters, data_num);

	list_insert(loop, loop_index, loop_randq, loop_num);
	
	return;
}

void cmd_randq7()
{
	short int loop_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	int rho_numerator_0=arg_int[0];
	int rho_denominator_0=arg_int[1];
	int rho_numerator_1=arg_int[2];
	int rho_denominator_1=arg_int[3];
	int rho_numerator_2=arg_int[4];
	int rho_denominator_2=arg_int[5];
	int rho_numerator_3=arg_int[6];
	int rho_denominator_3=arg_int[7];
	int rho_numerator_4=arg_int[8];
	int rho_denominator_4=arg_int[9];
	int rho_numerator_5=arg_int[10];
	int rho_denominator_5=arg_int[11];
	
	struct loop_randq_data randqdata;
	
	randqdata.data_index_parameters=data_index_parameters;
	randqdata.rho_numerator[0]=rho_numerator_0;
	randqdata.rho_denominator[0]=rho_denominator_0;
	randqdata.rho_numerator[1]=rho_numerator_1;
	randqdata.rho_denominator[1]=rho_denominator_1;
	randqdata.rho_numerator[2]=rho_numerator_2;
	randqdata.rho_denominator[2]=rho_denominator_2;
	randqdata.rho_numerator[3]=rho_numerator_3;
	randqdata.rho_denominator[3]=rho_denominator_3;
	randqdata.rho_numerator[4]=rho_numerator_4;
	randqdata.rho_denominator[4]=rho_denominator_4;
	randqdata.rho_numerator[5]=rho_numerator_5;
	randqdata.rho_denominator[5]=rho_denominator_5;

	loop_randq(command_insert, &randqdata, data, data_parameters, data_num);

	list_insert(loop, loop_index, loop_randq, loop_num);
	
	return;
}

void cmd_randq8()
{
	short int loop_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	int rho_numerator_0=arg_int[0];
	int rho_denominator_0=arg_int[1];
	int rho_numerator_1=arg_int[2];
	int rho_denominator_1=arg_int[3];
	int rho_numerator_2=arg_int[4];
	int rho_denominator_2=arg_int[5];
	int rho_numerator_3=arg_int[6];
	int rho_denominator_3=arg_int[7];
	int rho_numerator_4=arg_int[8];
	int rho_denominator_4=arg_int[9];
	int rho_numerator_5=arg_int[10];
	int rho_denominator_5=arg_int[11];
	int rho_numerator_6=arg_int[12];
	int rho_denominator_6=arg_int[13];
	
	struct loop_randq_data randqdata;
	
	randqdata.data_index_parameters=data_index_parameters;
	randqdata.rho_numerator[0]=rho_numerator_0;
	randqdata.rho_denominator[0]=rho_denominator_0;
	randqdata.rho_numerator[1]=rho_numerator_1;
	randqdata.rho_denominator[1]=rho_denominator_1;
	randqdata.rho_numerator[2]=rho_numerator_2;
	randqdata.rho_denominator[2]=rho_denominator_2;
	randqdata.rho_numerator[3]=rho_numerator_3;
	randqdata.rho_denominator[3]=rho_denominator_3;
	randqdata.rho_numerator[4]=rho_numerator_4;
	randqdata.rho_denominator[4]=rho_denominator_4;
	randqdata.rho_numerator[5]=rho_numerator_5;
	randqdata.rho_denominator[5]=rho_denominator_5;
	randqdata.rho_numerator[6]=rho_numerator_6;
	randqdata.rho_denominator[6]=rho_denominator_6;

	loop_randq(command_insert, &randqdata, data, data_parameters, data_num);

	list_insert(loop, loop_index, loop_randq, loop_num);
	
	return;
}

void cmd_randq9()
{
	short int loop_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	int rho_numerator_0=arg_int[0];
	int rho_denominator_0=arg_int[1];
	int rho_numerator_1=arg_int[2];
	int rho_denominator_1=arg_int[3];
	int rho_numerator_2=arg_int[4];
	int rho_denominator_2=arg_int[5];
	int rho_numerator_3=arg_int[6];
	int rho_denominator_3=arg_int[7];
	int rho_numerator_4=arg_int[8];
	int rho_denominator_4=arg_int[9];
	int rho_numerator_5=arg_int[10];
	int rho_denominator_5=arg_int[11];
	int rho_numerator_6=arg_int[12];
	int rho_denominator_6=arg_int[13];
	int rho_numerator_7=arg_int[14];
	int rho_denominator_7=arg_int[15];
	
	struct loop_randq_data randqdata;
	
	randqdata.data_index_parameters=data_index_parameters;
	randqdata.rho_numerator[0]=rho_numerator_0;
	randqdata.rho_denominator[0]=rho_denominator_0;
	randqdata.rho_numerator[1]=rho_numerator_1;
	randqdata.rho_denominator[1]=rho_denominator_1;
	randqdata.rho_numerator[2]=rho_numerator_2;
	randqdata.rho_denominator[2]=rho_denominator_2;
	randqdata.rho_numerator[3]=rho_numerator_3;
	randqdata.rho_denominator[3]=rho_denominator_3;
	randqdata.rho_numerator[4]=rho_numerator_4;
	randqdata.rho_denominator[4]=rho_denominator_4;
	randqdata.rho_numerator[5]=rho_numerator_5;
	randqdata.rho_denominator[5]=rho_denominator_5;
	randqdata.rho_numerator[6]=rho_numerator_6;
	randqdata.rho_denominator[6]=rho_denominator_6;
	randqdata.rho_numerator[7]=rho_numerator_7;
	randqdata.rho_denominator[7]=rho_denominator_7;

	loop_randq(command_insert, &randqdata, data, data_parameters, data_num);

	list_insert(loop, loop_index, loop_randq, loop_num);
	
	return;
}

void cmd_tsc()
{
	short int loop_index=arg_short_int[0];
	short int data_index=arg_short_int[1];

	struct loop_seed_tsc_data command_data;

	command_data.rng_struct=data[data_index];
	loop_seed_tsc(command_insert, &command_data, data, data_parameters, data_num);

	list_insert(loop, loop_index, loop_seed_tsc, loop_num);

	return;
}

