#include <mpi.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <chrono>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <cmath>
#include "wave.h"
#include "read_parameters.cpp"
using namespace std;

// initial-condition parameters for the problem (read from parameters.ini)
read_parameters parameters;
int out_cnt = 0;
int it = 0;
int num;
int idx;

// variables for timing
chrono::high_resolution_clock::time_point start;
chrono::high_resolution_clock::time_point finish;
std::chrono::duration<double> elapsed;

// some parameters for MPI communication
int id, p, tag_num = 1;
const double zero = 0;
int* start_indexs;
int* num_allocates;

int** receive_index;

// some MPI_Datatypes to build for communication
MPI_Datatype* Send_to_neighbour;
MPI_Datatype* Send_to_neighbour2;
MPI_Datatype* Send_to_neighbour3;

MPI_Aint** Send_to_neighbour_displacements;
MPI_Aint** Send_to_neighbour_displacements2;
MPI_Aint** Send_to_neighbour_displacements3;

MPI_Aint** Send_to_neighbour_address;
MPI_Aint** Send_to_neighbour_address2;
MPI_Aint** Send_to_neighbour_address3;

// MPI_Request for communication
MPI_Request* requests;
int cnt;

vector<int>* index_right_for_processor;
vector<int>* index_left_for_processor;
vector<int>* index_up_for_processor;
vector<int>* index_down_for_processor;

// some pointers storing the neighbours for grids
double** left_1;
double** left_2;
double** left_3;
double** right_1;
double** right_2;
double** right_3;
double** up_1;
double** up_2;
double** up_3;
double** down_1;
double** down_2;
double** down_3;


// MPI type use for output
MPI_Datatype UpEdge;
MPI_Datatype OutputType;
MPI_Datatype DownEdge;

// MPI type responsible for new_u and old_u
MPI_Datatype OutputType2;
MPI_Datatype OutputType3;

// start address for the object
MPI_Aint add_start;

wave_data::wave_data()
{
	index_right_for_processor = new vector<int>[p];
	index_left_for_processor = new vector<int>[p];
	index_up_for_processor = new vector<int>[p];
	index_down_for_processor = new vector<int>[p];

    Send_to_neighbour = new MPI_Datatype[p];
    Send_to_neighbour2 = new MPI_Datatype[p];
    Send_to_neighbour3 = new MPI_Datatype[p];

    Send_to_neighbour_displacements = new MPI_Aint*[p];
    Send_to_neighbour_displacements2 = new MPI_Aint*[p];
    Send_to_neighbour_displacements3 = new MPI_Aint*[p]; 

    Send_to_neighbour_address = new MPI_Aint*[p];
    Send_to_neighbour_address2 = new MPI_Aint*[p];
    Send_to_neighbour_address3 = new MPI_Aint*[p];

	send_length = new int[p];
    receive_from_neighbour = new double*[p];
	requests = new MPI_Request[2 * (p - 1)];

	receive_index = new int*[p];
}

wave_data::~wave_data()
{
    delete[] u;
	delete[] old_u;
	delete[] new_u;
	delete[] x_index;
	delete[] y_index;

	delete[] index_right_for_processor;
	delete[] index_left_for_processor;
	delete[] index_up_for_processor;
	delete[] index_down_for_processor;

    delete[] Send_to_neighbour;
    delete[] Send_to_neighbour2;
    delete[] Send_to_neighbour3;

    delete[] Send_to_neighbour_displacements;
    delete[] Send_to_neighbour_displacements2;
    delete[] Send_to_neighbour_displacements3; 

    delete[] Send_to_neighbour_address;
    delete[] Send_to_neighbour_address2;
    delete[] Send_to_neighbour_address3;

	delete[] send_length;
    delete[] receive_from_neighbour;
	delete[] requests;

    delete[] left_1;
    delete[] left_2;
    delete[] left_3;
    delete[] right_1;
    delete[] right_2;
    delete[] right_3;
    delete[] up_1;
    delete[] up_2;
    delete[] up_3;
    delete[] down_1;
    delete[] down_2;
    delete[] down_3;

}

// find which processor is responsible for this certain grid
int find_processor(int a)
{   
	int i = 1;

    while(i < p)
	{       
		if(start_indexs[i] > a) break;
		++i;
	}

	return i - 1;
}

// find neighbouring processors for current processor
void wave_data::find_neighbour_for_other_processor()
{   
	bool isolated;
	int proc;

    // setting right-neighbour for other processor, all boundary types fits
	if (id != 0) index_right_for_processor[id - 1].push_back(0);
	// setting left-neighbour for other processor, all boundary types fits
	if(id != p - 1) index_left_for_processor[id + 1].push_back(n - 1);

    // setting neighbour communication dataTypes
    for (int i = 0; i < n; ++i)
	{   

		if(id != 0 && i == 0) isolated = false;
		else if(id != p - 1 && i == n - 1) isolated = false;
		else 
		{   
			// if (id == 1) cout << i << " ";
			isolated = true;
		}

		if(parameters.boundary_type == "periodic")
		{  
		   // setting right-neighbour for other processor
           if(y_index[i] == 0)
		   {
			   if((proc = find_processor(i + start_index + jmax - 1))!= id)
			   {
                  index_right_for_processor[proc].push_back(i);
				  isolated = false;
			   }
			   else
			   {
                  left_1[i] = &u[i + jmax - 1];
			      left_2[i] = &new_u[i + jmax - 1];
			      left_3[i] = &old_u[i + jmax - 1];				   
			   }
		   }
		   else if(i != 0)
		   {
               left_1[i] = &u[i - 1];
			   left_2[i] = &new_u[i - 1];
			   left_3[i] = &old_u[i - 1];
		   }

		   // setting left-neighbour for other processor
           if(y_index[i] == jmax - 1)
		   {
			   if((proc = find_processor(i + start_index - jmax + 1))!= id)
			   {
                  index_left_for_processor[proc].push_back(i);
				  isolated = false;
			   }
			   else
			   {
                  right_1[i] = &u[i - jmax + 1];
			      right_2[i] = &new_u[i - jmax + 1];
			      right_3[i] = &old_u[i - jmax + 1];				   
			   }
		   }
		   else if(i != n - 1)
		   {
               right_1[i] = &u[i + 1];
			   right_2[i] = &new_u[i + 1];
			   right_3[i] = &old_u[i + 1];			   
		   }

		   // setting down-neighbour for other processor
		   if(x_index[i] == 0)
		   {
			   if((proc = find_processor(i + start_index + jmax * (imax - 1)))!= id)
			   {
                  index_down_for_processor[proc].push_back(i);
				  isolated = false;
			   }
		   }
		   else
		   {
			   if((proc = find_processor(i + start_index - jmax))!= id)
			   {
                  index_down_for_processor[proc].push_back(i);
				  isolated = false;
			   }
			   else
			   {
				    up_1[i] = &u[i - jmax];
			        up_2[i] = &new_u[i - jmax];
			        up_3[i] = &old_u[i - jmax];	
			   }
		   }

		   // setting up-neighbour for other processor
		   if(x_index[i] == imax - 1)
		   {
			   if((proc = find_processor(i + start_index - jmax * (imax - 1)))!= id)
			   {
                  index_up_for_processor[proc].push_back(i);
				  isolated = false;
			   }
		   }
		   else
		   {
			   if((proc = find_processor(i + start_index + jmax))!= id)
			   {  
                  index_up_for_processor[proc].push_back(i);
				  isolated = false;
			   }
			   else
			   {
				    down_1[i] = &u[i + jmax];
			        down_2[i] = &new_u[i + jmax];
			        down_3[i] = &old_u[i + jmax];	
			   }
		   }

		} 
		else
		{
            // setting down-neighbour for other processor
			if(x_index[i]!= 0)
			{   
				if((proc = find_processor(i + start_index - jmax))!= id)
				{
                    index_down_for_processor[proc].push_back(i);
				    isolated = false;
				}
			    else
			    {
				    up_1[i] = &u[i - jmax];
			        up_2[i] = &new_u[i - jmax];
			        up_3[i] = &old_u[i - jmax];	
			    }				
			}

		    // setting up-neighbour for other processor
			if(x_index[i]!= imax - 1)
			{   
				if ((proc = find_processor(i + start_index + jmax))!= id)
                {
					index_up_for_processor[proc].push_back(i);
				    isolated = false;
				}
				else
			    {
				    down_1[i] = &u[i + jmax];
			        down_2[i] = &new_u[i + jmax];
			        down_3[i] = &old_u[i + jmax];					
			    }

			}

			if(y_index[i] != 0  && i != 0)
			{
               left_1[i] = &u[i - 1];
			   left_2[i] = &new_u[i - 1];
			   left_3[i] = &old_u[i - 1];				
			}	

			if(y_index[i] != jmax - 1  && i != n - 1)	
            {
               right_1[i] = &u[i + 1];
			   right_2[i] = &new_u[i + 1];
			   right_3[i] = &old_u[i + 1];					
			}	
		}

		if(isolated) 
		{   
			index_isolated.push_back(i);
		}
		else
		{
			index_not_isolated.push_back(i);
		}
	}

	isolated_num = index_isolated.size();
	non_isolated_num = index_not_isolated.size();

}

// building as MPI_Datatypes for communication as well as a receive array
void wave_data::build_communication_type()
{   
	int com_len;
	int len_vec;

	for (int i = 0; i < p; i++)
	{   
		com_len = 0;
         
		if (i == id)  send_length[i] = 0;
        else
		{
			send_length[i] = index_right_for_processor[i].size() + index_left_for_processor[i].size() \
		  + index_down_for_processor[i].size() + index_up_for_processor[i].size();
		}

		if (send_length[i]!= 0)
		{
		   Send_to_neighbour_displacements[i] = new MPI_Aint[send_length[i]];
		   Send_to_neighbour_displacements2[i] = new MPI_Aint[send_length[i]];
		   Send_to_neighbour_displacements3[i] = new MPI_Aint[send_length[i]];
		   Send_to_neighbour_address[i] = new MPI_Aint[send_length[i]];
		   Send_to_neighbour_address2[i] = new MPI_Aint[send_length[i]];
		   Send_to_neighbour_address3[i] = new MPI_Aint[send_length[i]];
		   receive_from_neighbour[i] = new double[send_length[i]];
		   int block_lengths[send_length[i]];
		   MPI_Datatype typelist[send_length[i]];
           
		    len_vec = 0;
		    // setting DataType for sending to neighbours
            while(com_len < index_right_for_processor[i].size())
			{
                MPI_Get_address(&u[index_right_for_processor[i][len_vec]], &Send_to_neighbour_address[i][com_len]);
				MPI_Get_address(&new_u[index_right_for_processor[i][len_vec]], &Send_to_neighbour_address2[i][com_len]);
				MPI_Get_address(&old_u[index_right_for_processor[i][len_vec]], &Send_to_neighbour_address3[i][com_len]);
				left_1[index_right_for_processor[i][len_vec]] = &receive_from_neighbour[i][com_len + index_left_for_processor[i].size()];
				left_2[index_right_for_processor[i][len_vec]] = &receive_from_neighbour[i][com_len + index_left_for_processor[i].size()];
				left_3[index_right_for_processor[i][len_vec]] = &receive_from_neighbour[i][com_len + index_left_for_processor[i].size()];

				com_len++;
				len_vec++;
			}

			len_vec = 0;

            while(com_len < index_right_for_processor[i].size() + index_left_for_processor[i].size())
			{   

                MPI_Get_address(&u[index_left_for_processor[i][len_vec]], &Send_to_neighbour_address[i][com_len]);
				MPI_Get_address(&new_u[index_left_for_processor[i][len_vec]], &Send_to_neighbour_address2[i][com_len]);
				MPI_Get_address(&old_u[index_left_for_processor[i][len_vec]], &Send_to_neighbour_address3[i][com_len]);
				right_1[index_left_for_processor[i][len_vec]] = &receive_from_neighbour[i][com_len - index_right_for_processor[i].size()];
				right_2[index_left_for_processor[i][len_vec]] = &receive_from_neighbour[i][com_len - index_right_for_processor[i].size()];
				right_3[index_left_for_processor[i][len_vec]] = &receive_from_neighbour[i][com_len - index_right_for_processor[i].size()];
               
				com_len++;
				len_vec++;
			}
            
			len_vec = 0;

            while(com_len < index_right_for_processor[i].size() + index_left_for_processor[i].size() + index_down_for_processor[i].size())
			{
                MPI_Get_address(&u[index_down_for_processor[i][len_vec]], &Send_to_neighbour_address[i][com_len]);
				MPI_Get_address(&new_u[index_down_for_processor[i][len_vec]], &Send_to_neighbour_address2[i][com_len]);
				MPI_Get_address(&old_u[index_down_for_processor[i][len_vec]], &Send_to_neighbour_address3[i][com_len]);
				up_1[index_down_for_processor[i][len_vec]] = &receive_from_neighbour[i][com_len + index_up_for_processor[i].size()];
				up_2[index_down_for_processor[i][len_vec]] = &receive_from_neighbour[i][com_len + index_up_for_processor[i].size()];
				up_3[index_down_for_processor[i][len_vec]] = &receive_from_neighbour[i][com_len + index_up_for_processor[i].size()];

				com_len++;
				len_vec++;
			}

			len_vec = 0;

            while(com_len < send_length[i])
			{   
                MPI_Get_address(&u[index_up_for_processor[i][len_vec]], &Send_to_neighbour_address[i][com_len]);
				MPI_Get_address(&new_u[index_up_for_processor[i][len_vec]], &Send_to_neighbour_address2[i][com_len]);
				MPI_Get_address(&old_u[index_up_for_processor[i][len_vec]], &Send_to_neighbour_address3[i][com_len]);
				down_1[index_up_for_processor[i][len_vec]] = &receive_from_neighbour[i][com_len - index_down_for_processor[i].size()];
				down_2[index_up_for_processor[i][len_vec]] = &receive_from_neighbour[i][com_len - index_down_for_processor[i].size()];
				down_3[index_up_for_processor[i][len_vec]] = &receive_from_neighbour[i][com_len - index_down_for_processor[i].size()];

				com_len++;
				len_vec++;
			}			
            
			for (int j = 0; j < send_length[i]; ++j)
			{  
			   block_lengths[j] = 1;
			   typelist[j] = MPI_DOUBLE;
			   Send_to_neighbour_displacements[i][j] = Send_to_neighbour_address[i][j] - add_start;
			   Send_to_neighbour_displacements2[i][j] = Send_to_neighbour_address2[i][j] - add_start;
			   Send_to_neighbour_displacements3[i][j] = Send_to_neighbour_address3[i][j] - add_start;
			}

	       MPI_Type_create_struct(send_length[i], block_lengths, Send_to_neighbour_displacements[i], typelist, &Send_to_neighbour[i]);
	       MPI_Type_commit(&Send_to_neighbour[i]);
	       MPI_Type_create_struct(send_length[i], block_lengths, Send_to_neighbour_displacements2[i], typelist, &Send_to_neighbour2[i]);
	       MPI_Type_commit(&Send_to_neighbour2[i]);
	       MPI_Type_create_struct(send_length[i], block_lengths, Send_to_neighbour_displacements3[i], typelist, &Send_to_neighbour3[i]);
	       MPI_Type_commit(&Send_to_neighbour3[i]);	

		}
	}
}

void wave_data::set_Neumman_boundary_u()
{   
    // set up Neumman_boundary
	if(up_edge_length!= 0)
	{
      for (int i = 0; i < up_edge_length; ++i)
	  {    
		up_boundaries[i] =  u[index_connect_up_edge[i]];
	  }
	}

    // set left Neumman_boundary
	if(left_edge_length!= 0)
	{
      for (int i = 0; i < left_edge_length; ++i)
	  {    
		left_boundaries[i] =  u[index_connect_left_edge[i]];
	  }
	}

    // set right Neumman_boundary
	if(right_edge_length!= 0)
	{
      for (int i = 0; i < right_edge_length; ++i)
	  {    
		right_boundaries[i] =  u[index_connect_right_edge[i]];
	  }
	}

    // set down Neumman_boundary
	if(down_edge_length!= 0)
	{
      for (int i = 0; i < down_edge_length; ++i)
	  {    
		down_boundaries[i] =  u[index_connect_down_edge[i]];
	  }	
	}
}

void wave_data::set_Neumman_boundary_new_u()
{   
    // set up Neumman_boundary
	if(up_edge_length!= 0)
	{
      for (int i = 0; i < up_edge_length; ++i)
	  {    
		up_boundaries[i] =  new_u[index_connect_up_edge[i]];
	  }
	}

    // set left Neumman_boundary
	if(left_edge_length!= 0)
	{
      for (int i = 0; i < left_edge_length; ++i)
	  {    
		left_boundaries[i] =  new_u[index_connect_left_edge[i]];
	  }
	}

    // set right Neumman_boundary
	if(right_edge_length!= 0)
	{
      for (int i = 0; i < right_edge_length; ++i)
	  {    
		right_boundaries[i] =  new_u[index_connect_right_edge[i]];
	  }
	}

    // set down Neumman_boundary
	if(down_edge_length!= 0)
	{
      for (int i = 0; i < down_edge_length; ++i)
	  {    
		down_boundaries[i] =  new_u[index_connect_down_edge[i]];
	  }	
	}
}

void wave_data::set_Neumman_boundary_old_u()
{   
    // set up Neumman_boundary
	if(up_edge_length!= 0)
	{
      for (int i = 0; i < up_edge_length; ++i)
	  {    
		up_boundaries[i] =  old_u[index_connect_up_edge[i]];
	  }
	}

    // set left Neumman_boundary
	if(left_edge_length!= 0)
	{
      for (int i = 0; i < left_edge_length; ++i)
	  {    
		left_boundaries[i] =  old_u[index_connect_left_edge[i]];
	  }
	}

    // set right Neumman_boundary
	if(right_edge_length!= 0)
	{
      for (int i = 0; i < right_edge_length; ++i)
	  {    
		right_boundaries[i] =  old_u[index_connect_right_edge[i]];
	  }
	}

    // set down Neumman_boundary
	if(down_edge_length!= 0)
	{
      for (int i = 0; i < down_edge_length; ++i)
	  {    
		down_boundaries[i] =  old_u[index_connect_down_edge[i]];
	  }	
	}
}


void wave_data::do_communication()
{    	 
    
	cnt = 0; 
	if (num == 0)
	{
	    for (int i = 0; i < p; ++i)
	    {   
		    if (i != id)
		    {	
			   if (send_length[i] != 0)
			   {		
			   // receive from other processors
               MPI_Irecv(receive_from_neighbour[i], send_length[i], MPI_DOUBLE, i, tag_num, MPI_COMM_WORLD, &requests[cnt]);
			   cnt++;	

			   // send to other processors		   		   
               MPI_Isend(this, 1, Send_to_neighbour[i], i, tag_num, MPI_COMM_WORLD, &requests[cnt]);
			   cnt++;
			   }
		    }		
	    }
	}  
	else if (num == 1)
	{
	    for (int i = 0; i < p; ++i)
	    {   
		    if (i != id)
		    {				
			   if (send_length[i] != 0)
			   {
			   // receive from other processors
               MPI_Irecv(receive_from_neighbour[i], send_length[i], MPI_DOUBLE, i, tag_num, MPI_COMM_WORLD, &requests[cnt]);
			   cnt++;

               // send to other processors
               MPI_Isend(this, 1, Send_to_neighbour2[i], i, tag_num, MPI_COMM_WORLD, &requests[cnt]);
			   cnt++;
			   }
		    }		
	    }
	}    
	else
	{
	    for (int i = 0; i < p; ++i)
	    {   
		    if (i != id)
		    {	
			   if (send_length[i] != 0)
			   {				
			   // receive from other processors
               MPI_Irecv(receive_from_neighbour[i], send_length[i], MPI_DOUBLE, i, tag_num, MPI_COMM_WORLD, &requests[cnt]);
			   cnt++;

			   // send to other processors
               MPI_Isend(this, 1, Send_to_neighbour3[i], i, tag_num, MPI_COMM_WORLD, &requests[cnt]);
			   cnt++;
			   }
		    }		
	    }
	}    
}

// setting up parameters for the wave equation.
void wave_data::setting_paramters()
{  
    imax = parameters.imax;
	jmax = parameters.jmax;

	// only interior grid for Neumann and Dirichlet
	if (parameters.boundary_type != "periodic")
	{
		imax-= 2;
		jmax-= 2;
	}

    t_max = parameters.t_end;
	dt_out = parameters.t_out;
	dt = parameters.dt;
	x_max = parameters.length;
    y_max = parameters.width;
    c = parameters.c;
	display = parameters.display_option;
	output = parameters.output;
	t_out = dt_out;

	
	dx = x_max / ((double)imax - 1);
	dy = y_max / ((double)jmax - 1);
    
	// checking if time step dt is too big
	if (dt > 0.1 * min(dx, dy) / c )
	{  
	   if (id == 0) cerr << "Error :\n dt is not less or equal to min(dx, dy) / c , the numerical solution would be unstable !!!! " << endl;
	   exit(EXIT_FAILURE);
	}

	mod = (imax * jmax) % p;
    intdiv = (imax * jmax) / p;
}

// allocate the grid to different processors
void wave_data::allocate_grid()
{   
	n = intdiv + (id < mod);
	start_index = id * (id < mod) + id * intdiv + mod * (id >= mod);
	end_index = start_index + n - 1;

	start_indexs = new int[p];
	num_allocates = new int[p];


    for (int i = 0; i < p; ++i)
	{
           num_allocates[i] = intdiv + (i < mod);
		   start_indexs[i] = i * (i < mod) + i * intdiv + mod * (i >= mod);
	}

	MPI_Get_address(this, &add_start);

	u = new double[n];
	old_u = new double[n];
	new_u = new double[n];

    left_1 = new double*[n];
    left_2 = new double*[n];
    left_3 = new double*[n];
    right_1 = new double*[n];  
	right_2 = new double*[n];
    right_3 = new double*[n];
    up_1 = new double*[n];
    up_2 = new double*[n];
    up_3 = new double*[n];
    down_1 = new double*[n];
    down_2 = new double*[n];
    down_3 = new double*[n];


	x_index = new int[n];
	y_index = new int[n];

	for (int i = 0; i < n; ++i)
	{   
		x_index[i] = (start_index + i) / jmax;
		y_index[i] = (start_index + i) % jmax;
	}

    line_start = (x_index[0] + 1) * jmax;
	line_end = (x_index[n - 1]) * jmax;
	n_line = x_index[n - 1] - x_index[0];


	for (int i = 0; i < n; ++i)
	{
		u[i] = 0;
		old_u[i] = 0;
        new_u[i] = 0;
	    // allocate boundary grids to each processor, for non-periodic condition
	    if (parameters.boundary_type != "periodic")
	    {   
			if (x_index[i] == 0) index_connect_up_edge.push_back(i);
            if (x_index[i] == imax - 1) index_connect_down_edge.push_back(i);
			if (y_index[i] == jmax - 1) index_connect_right_edge.push_back(i);
            if (y_index[i] == 0) index_connect_left_edge.push_back(i);
	    }
	}

	// allocate boundary grids to each processor, for non-periodic condition
	if (parameters.boundary_type != "periodic")
	{   
		up_edge_length = index_connect_up_edge.size();
		left_edge_length = index_connect_left_edge.size();
		right_edge_length = index_connect_right_edge.size();
		down_edge_length = index_connect_down_edge.size();
      
		if(up_edge_length != 0) up_boundaries = new double[up_edge_length];
		if(left_edge_length != 0) left_boundaries = new double[left_edge_length];
		if(right_edge_length != 0)  right_boundaries = new double[right_edge_length];
		if(down_edge_length != 0) down_boundaries = new double[down_edge_length];

		//initialize boundaries
		for(int i = 0; i < up_edge_length; ++i) 
		{
			up_boundaries[i] = 0;
			up_1[index_connect_up_edge[i]] = &up_boundaries[i];
			up_2[index_connect_up_edge[i]] = &up_boundaries[i];
			up_3[index_connect_up_edge[i]] = &up_boundaries[i];
		}
		for(int i = 0; i < down_edge_length; ++i) 
		{
			down_boundaries[i] = 0;
			down_1[index_connect_down_edge[i]] = &down_boundaries[i];
			down_2[index_connect_down_edge[i]] = &down_boundaries[i];
			down_3[index_connect_down_edge[i]] = &down_boundaries[i];
		}
		for(int i = 0; i < left_edge_length; ++i) 
		{
			left_boundaries[i] = 0;
			left_1[index_connect_left_edge[i]] = &left_boundaries[i];
			left_2[index_connect_left_edge[i]] = &left_boundaries[i];
			left_3[index_connect_left_edge[i]] = &left_boundaries[i];
		}
		for(int i = 0; i < right_edge_length; ++i) 
		{
			right_boundaries[i] = 0;
            right_1[index_connect_right_edge[i]] = &right_boundaries[i];
			right_2[index_connect_right_edge[i]] = &right_boundaries[i];
			right_3[index_connect_right_edge[i]] = &right_boundaries[i];
		}

	}
}

void wave_data::build_OutputType()
{   
	if(parameters.boundary_type == "periodic")
	{
		int block_lengths[1];
	    MPI_Aint address[1];
	    MPI_Aint address2[1];
	    MPI_Aint address3[1];
	    MPI_Aint displacements[1];
	    MPI_Aint displacements2[1];
	    MPI_Aint displacements3[1];
		MPI_Datatype typelist[1];

		MPI_Get_address(&u[0], &address[0]);
		MPI_Get_address(&new_u[0], &address2[0]);
		MPI_Get_address(&old_u[0], &address3[0]);

		block_lengths[0] = n;
		typelist[0] = MPI_DOUBLE;

		displacements[0] = address[0] - add_start;
		displacements2[0] = address2[0] - add_start;
		displacements3[0] = address3[0] - add_start;

       	MPI_Type_create_struct(1, block_lengths, displacements, typelist, &OutputType);
	    MPI_Type_commit(&OutputType);
	    MPI_Type_create_struct(1, block_lengths, displacements2, typelist, &OutputType2);
	    MPI_Type_commit(&OutputType2);
	    MPI_Type_create_struct(1, block_lengths, displacements3, typelist, &OutputType3);
	    MPI_Type_commit(&OutputType3);
	}
	else
	{   
		// build up & down output type
	    left_up_corner = false;
	    right_up_corner = false;
	    left_down_corner = false;
	    right_down_corner = false;


		if (up_edge_length != 0)
		{   
			if(y_index[index_connect_up_edge[0]] == 0) left_up_corner = true;
		    if (y_index[index_connect_up_edge[up_edge_length - 1]] == jmax - 1)  right_up_corner = true;
		}
		if (down_edge_length!= 0)
		{   
			if(y_index[index_connect_down_edge[0]] == 0) left_down_corner = true;
		    if(y_index[index_connect_down_edge[down_edge_length - 1]] == jmax - 1) right_down_corner = true;
		}


		if (!left_up_corner && !right_up_corner)
	      {
	          int block_lengths[1];
	          MPI_Aint address[1];
	          MPI_Aint displacements[1];
              MPI_Datatype typelist[1];
              
			  block_lengths[0] = up_edge_length;
			  MPI_Get_address(up_boundaries, &address[0]);
	          displacements[0] = address[0] - add_start;
			  typelist[0] = MPI_DOUBLE;

	          MPI_Type_create_struct(1, block_lengths, displacements, typelist, &UpEdge);
	          MPI_Type_commit(&UpEdge);
		  }
	      else if (left_up_corner && !right_up_corner)
	      {
	          int block_lengths[2];
	          MPI_Aint address[2];
	          MPI_Aint displacements[2];
              MPI_Datatype typelist[2];
              
			  block_lengths[0] = 1;
			  block_lengths[1] = up_edge_length;
			  MPI_Get_address(&zero, &address[0]);
			  MPI_Get_address(up_boundaries, &address[1]);              
	          for (int i = 0; i < 2; i++) displacements[i] = address[i] - add_start;
			  for (int i = 0; i < 2; i++) typelist[i] = MPI_DOUBLE;

	          MPI_Type_create_struct(2, block_lengths, displacements, typelist, &UpEdge);
	          MPI_Type_commit(&UpEdge);
	      }
	      else if(right_up_corner && !left_up_corner)
	      {
	          int block_lengths[2];
	          MPI_Aint address[2];
	          MPI_Aint displacements[2];
              MPI_Datatype typelist[2];
              
			  block_lengths[1] = 1;
			  block_lengths[0] = up_edge_length;
			  MPI_Get_address(&zero, &address[1]);
			  MPI_Get_address(up_boundaries, &address[0]);              
	          for (int i = 0; i < 2; i++) displacements[i] = address[i] - add_start;
			  for (int i = 0; i < 2; i++) typelist[i] = MPI_DOUBLE;

	          MPI_Type_create_struct(2, block_lengths, displacements, typelist, &UpEdge);
	          MPI_Type_commit(&UpEdge);          
	      }	
	    else
	      {
	          int block_lengths[3];
	          MPI_Aint address[3];
	          MPI_Aint displacements[3];
              MPI_Datatype typelist[3];
              
			  block_lengths[0] = 1;
			  block_lengths[1] = up_edge_length;
			  block_lengths[2] = 1;
			  MPI_Get_address(&zero, &address[0]);
			  MPI_Get_address(up_boundaries, &address[1]);
			  MPI_Get_address(&zero, &address[2]);          
	          for (int i = 0; i < 3; i++) displacements[i] = address[i] - add_start;
			  for (int i = 0; i < 3; i++) typelist[i] = MPI_DOUBLE;

	          MPI_Type_create_struct(3, block_lengths, displacements, typelist, &UpEdge);
	          MPI_Type_commit(&UpEdge);    
	      }
	    


	  if(!left_down_corner && !right_down_corner)
	  {
	        int block_lengths[1];
	        MPI_Aint address[1];
	        MPI_Aint displacements[1];
            MPI_Datatype typelist[1];
              
			block_lengths[0] = down_edge_length;
			MPI_Get_address(down_boundaries, &address[0]);
	        displacements[0] = address[0] - add_start;
			typelist[0] = MPI_DOUBLE;

	        MPI_Type_create_struct(1, block_lengths, displacements, typelist, &DownEdge);
	        MPI_Type_commit(&DownEdge);     
	  }
	  else if (left_down_corner && !right_down_corner)
	  {
	        int block_lengths[2];
	        MPI_Aint address[2];
	        MPI_Aint displacements[2];
            MPI_Datatype typelist[2];
              
			block_lengths[0] = 1;
			block_lengths[1] = down_edge_length;
			MPI_Get_address(&zero, &address[0]);
			MPI_Get_address(down_boundaries, &address[1]);              
	        for (int i = 0; i < 2; i++) displacements[i] = address[i] - add_start;
			for (int i = 0; i < 2; i++) typelist[i] = MPI_DOUBLE;

	        MPI_Type_create_struct(2, block_lengths, displacements, typelist, &DownEdge);
	        MPI_Type_commit(&DownEdge);
	  }
	  else if(right_down_corner && !left_down_corner)
	  {
	        int block_lengths[2];
	        MPI_Aint address[2];
	        MPI_Aint displacements[2];
            MPI_Datatype typelist[2];
              
			block_lengths[0] = 1;
			block_lengths[1] = down_edge_length;
			MPI_Get_address(&zero, &address[0]);
			MPI_Get_address(down_boundaries, &address[1]);              
	        for (int i = 0; i < 2; i++) displacements[i] = address[i] - add_start;
			for (int i = 0; i < 2; i++) typelist[i] = MPI_DOUBLE;

	        MPI_Type_create_struct(2, block_lengths, displacements, typelist, &DownEdge);
	        MPI_Type_commit(&DownEdge);
	  }	  
	  else
	  {
	        int block_lengths[3];
	        MPI_Aint address[3];
	        MPI_Aint displacements[3];
            MPI_Datatype typelist[3];
              
			block_lengths[0] = 1;
			block_lengths[1] = down_edge_length;
			block_lengths[2] = 1;
			MPI_Get_address(&zero, &address[0]);
			MPI_Get_address(down_boundaries, &address[1]);
			MPI_Get_address(&zero, &address[2]);          
	        for (int i = 0; i < 3; i++) displacements[i] = address[i] - add_start;
			for (int i = 0; i < 3; i++) typelist[i] = MPI_DOUBLE;

	        MPI_Type_create_struct(3, block_lengths, displacements, typelist, &DownEdge);
	        MPI_Type_commit(&DownEdge);  
	  }
	

	// build middle output type
	line_feeds = 3 * n_line + 1;
	line_feeds += (y_index[0] == 0) + (y_index[n - 1] == jmax - 1);
	int block_lengths[line_feeds];
	MPI_Aint address[line_feeds];
	MPI_Aint address2[line_feeds];
	MPI_Aint address3[line_feeds];
	MPI_Aint displacements[line_feeds];
	MPI_Aint displacements2[line_feeds];
	MPI_Aint displacements3[line_feeds];
    MPI_Datatype typelist[line_feeds];
	int cnt = 0;
	int right_count = 0;
	int left_count = 0;

	if(y_index[0] == 0) 
	{
		block_lengths[0] = 1;
		MPI_Get_address(&left_boundaries[0], &address[0]);
		MPI_Get_address(&left_boundaries[0], &address2[0]);
		MPI_Get_address(&left_boundaries[0], &address3[0]);
		typelist[0] = MPI_DOUBLE;
	    block_lengths[1] = line_start - start_index;
	    MPI_Get_address(&u[0], &address[1]);
	    MPI_Get_address(&new_u[0], &address2[1]);
	    MPI_Get_address(&old_u[0], &address3[1]);
		typelist[1] = MPI_DOUBLE;
		cnt += 2;
		++left_count;
	}
	else
	{
	    block_lengths[0] = line_start - start_index;
	    MPI_Get_address(&u[0], &address[0]);
	    MPI_Get_address(&new_u[0], &address2[0]);
	    MPI_Get_address(&old_u[0], &address3[0]);
		typelist[0] = MPI_DOUBLE;
		++cnt;
	}

	while(right_count < n_line - 1)
	{
		block_lengths[cnt] = 1;
		typelist[cnt] = MPI_DOUBLE;
        MPI_Get_address(&right_boundaries[right_count], &address[cnt]);
        MPI_Get_address(&right_boundaries[right_count], &address2[cnt]);
        MPI_Get_address(&right_boundaries[right_count], &address3[cnt]);
		++cnt;
		++right_count;

		block_lengths[cnt] = 1;
		typelist[cnt] = MPI_DOUBLE;
		MPI_Get_address(&left_boundaries[left_count], &address[cnt]);
		MPI_Get_address(&left_boundaries[left_count], &address2[cnt]);
		MPI_Get_address(&left_boundaries[left_count], &address3[cnt]);
		++cnt;	
		++left_count;

		block_lengths[cnt] = jmax;
		typelist[cnt] = MPI_DOUBLE;
		MPI_Get_address(&u[line_start - start_index], &address[cnt]);
		MPI_Get_address(&new_u[line_start - start_index], &address2[cnt]);
		MPI_Get_address(&old_u[line_start - start_index], &address3[cnt]);
		line_start += jmax;
		++cnt;
	}

	block_lengths[cnt] = 1;
	typelist[cnt] = MPI_DOUBLE;
    MPI_Get_address(&right_boundaries[right_count], &address[cnt]);
    MPI_Get_address(&right_boundaries[right_count], &address2[cnt]);
    MPI_Get_address(&right_boundaries[right_count], &address3[cnt]);
	++cnt;
	++right_count;

	block_lengths[cnt] = 1;
	typelist[cnt] = MPI_DOUBLE;
	MPI_Get_address(&left_boundaries[left_count], &address[cnt]);
	MPI_Get_address(&left_boundaries[left_count], &address2[cnt]);
	MPI_Get_address(&left_boundaries[left_count], &address3[cnt]);
	++cnt;	
	++left_count;

	block_lengths[cnt] = start_index + n - line_end;
	MPI_Get_address(&u[line_start - start_index], &address[cnt]);
	MPI_Get_address(&new_u[line_start - start_index], &address2[cnt]);
	MPI_Get_address(&old_u[line_start - start_index], &address3[cnt]);
	typelist[cnt] = MPI_DOUBLE;
    ++cnt;

	if(y_index[n - 1] == jmax - 1) 
	{
		block_lengths[cnt] = 1;
	    typelist[cnt] = MPI_DOUBLE;
        MPI_Get_address(&right_boundaries[right_count], &address[cnt]);
        MPI_Get_address(&right_boundaries[right_count], &address2[cnt]);
        MPI_Get_address(&right_boundaries[right_count], &address3[cnt]);		
	    ++cnt;
	    ++right_count;
	}

	for (int i = 0; i < line_feeds; ++i) 
	{
		displacements[i] = address[i] - add_start;
		displacements2[i] = address2[i] - add_start;
		displacements3[i] = address3[i] - add_start;
	}

	MPI_Type_create_struct(line_feeds, block_lengths, displacements, typelist, &OutputType);
	MPI_Type_commit(&OutputType);
	MPI_Type_create_struct(line_feeds, block_lengths, displacements2, typelist, &OutputType2);
	MPI_Type_commit(&OutputType2);
	MPI_Type_create_struct(line_feeds, block_lengths, displacements3, typelist, &OutputType3);
	MPI_Type_commit(&OutputType3);
	}
}

// setting initial splashes in parallel
void wave_data::set_initial_condition()
{   
	double dist, x, y, h, dist_x, dist_y;
	//sets half sinusoidal intitial disturbance
	for (int i = 0; i < n; i++)
	{    
		x = dx * x_index[i];
		y = dy * y_index[i];

        if (parameters.boundary_type == "periodic")
		{   
 		    for (int k = 0; k < parameters.num_splash; k++)
		    {       
			    dist_x = min(fabs(x - parameters.xs[k]), fabs(parameters.xs[k] + x_max - x));
				dist_x = min(dist_x, fabs(parameters.xs[k] - x_max - x));
				dist_y = min(fabs(y - parameters.ys[k]), fabs(parameters.ys[k] + y_max - y));
				dist_y = min(dist_y, fabs(parameters.ys[k] - y_max - y));				

			    dist = sqrt(pow(dist_x, 2.0) + pow(dist_y, 2.0));

		    if (dist < parameters.rs[k])
		    {
				h = 5.0*(cos(dist / parameters.rs[k] * M_PI) + 1.0);
				u[i] += h;
				old_u[i] += h;
		    }
		    }
		}
        else
		{   // only for the interior for Neumann and Dirichlet
		    for (int k = 0; k < parameters.num_splash; k++)
		    {   
			    dist = sqrt(pow(x - parameters.xs[k], 2.0) + pow(y - parameters.ys[k], 2.0));
		    if (dist < parameters.rs[k])
		    {
				h = 5.0*(cos(dist / parameters.rs[k] * M_PI) + 1.0);

				u[i] += h;
				old_u[i] += h;
		    }
			}
		}	
	}

    // the boundary setting for Neumann condition (boundary = nearest neighbour)
	if (parameters.boundary_type == "Neumann")
	{   
		set_Neumman_boundary_u();
	}
    
    // build output types for MPI_File_write
	build_OutputType();
}

void wave_data::MPI_grid_to_file()
{   
	MPI_File mpi_file;
	stringstream fname;
	fname << "./output/output" << "_" << out_cnt;    
	if (parameters.boundary_type == "periodic")
	{
        if (MPI_File_open(MPI_COMM_WORLD, fname.str().c_str(), MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &mpi_file) != MPI_SUCCESS)
	    {
		cout << "Error opening file!" << endl;
		exit(0);
	    }
		if(num == 0) MPI_File_write_ordered(mpi_file, this, 1, OutputType, MPI_STATUS_IGNORE);
		else if(num == 1) MPI_File_write_ordered(mpi_file, this, 1, OutputType2, MPI_STATUS_IGNORE);
		else if(num == 2) MPI_File_write_ordered(mpi_file, this, 1, OutputType3, MPI_STATUS_IGNORE);		
	}
	else
	{
	    if (MPI_File_open(MPI_COMM_WORLD, fname.str().c_str(), MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &mpi_file) != MPI_SUCCESS)
	    {
		cout << "Error opening file!" << endl;
		exit(0);
	    }

        if (num == 0)
		{  
           //currently writing u
           MPI_File_write_ordered(mpi_file, this, 1, UpEdge, MPI_STATUS_IGNORE);
	       MPI_File_write_ordered(mpi_file, this, 1, OutputType, MPI_STATUS_IGNORE);
           MPI_File_write_ordered(mpi_file, this, 1, DownEdge, MPI_STATUS_IGNORE);
		}
		else if(num == 1)
		{
		   //currently writing new_u
           MPI_File_write_ordered(mpi_file, this, 1, UpEdge, MPI_STATUS_IGNORE);
	       MPI_File_write_ordered(mpi_file, this, 1, OutputType2, MPI_STATUS_IGNORE);
           MPI_File_write_ordered(mpi_file, this, 1, DownEdge, MPI_STATUS_IGNORE);			
		}
		else
		{  
		   //currently writing old_u
           MPI_File_write_ordered(mpi_file, this, 1, UpEdge, MPI_STATUS_IGNORE);
	       MPI_File_write_ordered(mpi_file, this, 1, OutputType3, MPI_STATUS_IGNORE);
           MPI_File_write_ordered(mpi_file, this, 1, DownEdge, MPI_STATUS_IGNORE);				
		}

	}

	MPI_File_close(&mpi_file);
}

void wave_data::do_iteration()
{    
	 // remainder calculation to decide to update u, new_u or old_u
	 num = it % 3;    

	 // non-blocking communication, allow grids send value to its neighbour over different processors.
	 do_communication();

	 if(num == 1)
	 {  
		// calculate next stage for isolated grids
		for(int i = 0; i < isolated_num; ++i)
		{   
			idx = index_isolated[i];
            new_u[idx] = pow(dt * c, 2.0) * ((*right_1[idx] - 2.0 * u[idx] + *left_1[idx]) / pow(dx, 2.0) + (*down_1[idx] - 2.0 * u[idx] + *up_1[idx]) / pow(dy, 2.0)) + 2.0 * u[idx] - old_u[idx];
		}
        
		// wait the communications to be finished 
		MPI_Waitall(cnt, requests, MPI_STATUS_IGNORE);

		// calculate next stage for non-isolated grids
		for(int i = 0; i < non_isolated_num; ++i)
		{   
			idx = index_not_isolated[i];
            new_u[idx] = pow(dt * c, 2.0) * ((*right_1[idx] - 2.0 * u[idx] + *left_1[idx]) / pow(dx, 2.0) + (*down_1[idx] - 2.0 * u[idx] + *up_1[idx]) / pow(dy, 2.0)) + 2.0 * u[idx] - old_u[idx];
		} 
        
        // don't forgot to set Neumann boundary!
	    if (parameters.boundary_type == "Neumann") set_Neumman_boundary_new_u();        
	 }
	 else if (num == 2)
	 {  
		// calculate next stage for isolated grids
		for(int i = 0; i < isolated_num; ++i)
		{   
			idx = index_isolated[i];
            old_u[idx] = pow(dt * c, 2.0) * ((*right_2[idx] - 2.0 * new_u[idx] + *left_2[idx]) / pow(dx, 2.0) + (*down_2[idx] - 2.0 * new_u[idx] + *up_2[idx]) / pow(dy, 2.0)) + 2.0 * new_u[idx] - u[idx];
		}

        // wait the communications to be finished 
		MPI_Waitall(cnt, requests, MPI_STATUS_IGNORE);

		// calculate next stage for non-isolated grids		
		for(int i = 0; i < non_isolated_num; ++i)
		{   
			idx = index_not_isolated[i];
            old_u[idx] = pow(dt * c, 2.0) * ((*right_2[idx] - 2.0 * new_u[idx] + *left_2[idx]) / pow(dx, 2.0) + (*down_2[idx] - 2.0 * new_u[idx] + *up_2[idx]) / pow(dy, 2.0)) + 2.0 * new_u[idx] - u[idx];
		}

        // don't forgot to set Neumann boundary!
	    if (parameters.boundary_type == "Neumann") set_Neumman_boundary_old_u(); 
	 }
	 else
	 {
		// calculate next stage for isolated grids
		for(int i = 0; i < isolated_num; ++i)
		{   
			idx = index_isolated[i];
            u[idx] = pow(dt * c, 2.0) * ((*right_3[idx] - 2.0 * old_u[idx] + *left_3[idx]) / pow(dx, 2.0) + (*down_3[idx] - 2.0 * old_u[idx] + *up_3[idx]) / pow(dy, 2.0)) + 2.0 * old_u[idx] - new_u[idx];
		}

        // wait the communications to be finished 
		MPI_Waitall(cnt, requests, MPI_STATUS_IGNORE);

		// calculate next stage for non-isolated grids			
		for(int i = 0; i < non_isolated_num; ++i)
		{   
			idx = index_not_isolated[i];
            u[idx] = pow(dt * c, 2.0) * ((*right_3[idx] - 2.0 * old_u[idx] + *left_3[idx]) / pow(dx, 2.0) + (*down_3[idx] - 2.0 * old_u[idx] + *up_3[idx]) / pow(dy, 2.0)) + 2.0 * old_u[idx] - new_u[idx];
		}  

        // don't forgot to set Neumann boundary!
	    if (parameters.boundary_type == "Neumann") set_Neumman_boundary_u();
	 }


	 t += dt;
}

void wave_data::run_simulation()
{   
	// first frame
	if (output) 
	{
		MPI_grid_to_file();
		out_cnt++;
	    if (display)
	    {
		if (id == 0) cout << "output: " << out_cnt << "\tt: " << t << "\titeration: " << it << endl;
	    }
	}

    //do simulation until the end_time is achieved.
	while (t < t_max)
	{   
		it++;
		do_iteration();
        
	if (output)
	{
		if (t_out <= t)
		{   

			if(display)
			{
			    if (id == 0) cout << "output: " << out_cnt << "\tt: " << t << "\titeration: " << it << endl;
			}

			MPI_grid_to_file();
            out_cnt++;
			t_out += dt_out;
		}
	}
	
		

	}


}