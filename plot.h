/**************************************************************
***************************************************************
*** 
***   plot.h
***
***   Jason Olejarz
***   2011-01-27
***
***   This is the header file for plot.c.
***
***************************************************************
**************************************************************/

void animate_real_time(short int *spins, int *flips, double *energy, double *time, int steps_begin, int steps, double time_between_frames, int nx, int ny, double delay, double time_offset, short int save_frames, char matlab_data_filename[], FILE *matlab_data, FILE *matlab_plot);
void animate_real_time_2(short int *spins, int *flips, double *energy, double *time, int steps, double starting_time, double time_between_frames, int nx, int ny, double delay, short int save_frames, char matlab_data_filename[], FILE *matlab_data, FILE *matlab_plot);
void animate_step(short int *spins, int *flips, int *flips_backward, int steps, int nx, int ny, int xmin, int xmax, int ymin, int ymax, char matlab_data_filename[], FILE *matlab_data, FILE *matlab_plot);
void blinker_region_plot_matlab(struct run_parameters *p, char matlab_data_filename_1[], FILE *matlab_data_1, FILE *matlab_plot_1, char matlab_data_filename_2[], FILE *matlab_data_2, FILE *matlab_plot_2, char matlab_data_filename_3[], FILE *matlab_data_3, FILE *matlab_plot_3);
void cluster_spins_plot(short int *cluster_spins, int nx, int ny, int nz, char matlab_data_filename[], FILE *matlab_data, FILE *matlab_plot);
void state_plot_matlab(short int *spins, int nx, int ny, int steps, char matlab_data_filename[], FILE *matlab_data, FILE *matlab_plot);
