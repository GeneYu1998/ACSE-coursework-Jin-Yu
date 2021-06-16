#pragma once
#include "read_parameters.h"

class wave_data
{
public:
    // three arrays storing the time step
    double *u; 
    double *old_u;
    double *new_u;
    
	// these variables stores the parameter settings read from 'parameters.ini'
    read_parameters parameters;
	int imax, jmax;
    double t_max;
    double t = 0.0;
    double t_out = 0.0; 
    double dt_out, dt;
    double y_max, x_max, dx, dy;
    double c;
	bool display;
	bool output;

    // other useful variables used for allocating grid / building communication
	int n, intdiv, mod, start_index, end_index;
	int *x_index;
    int *y_index;
	int line_feeds;
	int line_start;
	int line_end;
	int n_line;
	int isolated_num;
	int non_isolated_num;
	int* block_lengths;


    // member functions
	wave_data();
    ~wave_data();
	void build_OutputType();
	void setting_paramters();
	void allocate_grid();
	void set_initial_condition();
	void MPI_grid_to_file();
	void set_Neumman_boundary_u();
	void set_Neumman_boundary_new_u();
	void set_Neumman_boundary_old_u();
	void find_neighbour_for_other_processor();
	void build_communication_type();
	void do_communication();
    void do_iteration();
	void run_simulation();


	// some vector containers for Neumann boundary
	vector<int> index_connect_up_edge;
	vector<int> index_connect_down_edge;
	vector<int> index_connect_left_edge;
	vector<int> index_connect_right_edge;
    
	// length of data connecting to boundaries (Neumann and Dirichlet)
	int up_edge_length;
	int down_edge_length;
	int left_edge_length;
	int right_edge_length;
    
	// some arrays for Neumann boundary
	double* up_boundaries;
	double* down_boundaries;
	double* left_boundaries;
	double* right_boundaries;
    
	// some arrays for recieve data from other processor
	int* send_length;
	double** receive_from_neighbour;


	// find index which isolated, or say, all neighbours for this node is located the current processor.
	vector<int> index_isolated;

    // find index which isolated, or say, this node is a neighbour of some other processor. 
	vector<int> index_not_isolated;

	// some booleans decide whether corner is near this node.
	bool left_up_corner;
	bool right_up_corner;
	bool left_down_corner;
	bool right_down_corner;



};