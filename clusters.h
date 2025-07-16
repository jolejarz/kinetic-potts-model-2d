/**************************************************************
***************************************************************
*** 
***   clusters.h
***
***   Jason Olejarz
***   2010-08-18
***
***   This is the header file for cluster.c.
***
***************************************************************
**************************************************************/

struct cluster_stats
{
	int label;
	int number_of_spins;
	short int sign;
	int perimeter;
	int winding_a;
	int winding_b;
};

struct cluster_topology
{
	int vertices;
	int edges;
	int faces;
	int euler_characteristic;
};

void clusters (struct run_parameters *parameters, int *lattice, short int *spins);
void clusters_percolation (struct run_parameters *parameters, int *lattice, short int *spins, int *displacement_x, int *displacement_y, int *winding_a_lattice, int *winding_b_lattice, int *stripes_count, int *winding_a_max, int *winding_b_max);
void cluster_stats (struct run_parameters *parameters, struct cluster_stats *cluster_list, int *lattice, short int *spins);
void cluster_stats_percolation (struct run_parameters *parameters, struct cluster_stats *cluster_list, int *lattice, short int *spins, int *winding_a_lattice, int *winding_b_lattice);
void save_cluster_spins (struct run_parameters *parameters, short int *cluster_spins, int cluster_number, struct cluster_stats *cluster_list, int *lattice);
