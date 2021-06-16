# include "wave.cpp"

int main(int argc, char *argv[])
{   
	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
    
	// create an object
	wave_data wave;
	// initial settings
	wave.setting_paramters();

	if(!wave.display || !wave.output)
	{
		if(id == 0)  start = chrono::high_resolution_clock::now();
	}

    // allocate grids for processors
	wave.allocate_grid();
	// setting initial conditions (in parallel)
	wave.set_initial_condition();
	// find neighbouring processor for the grids in one specific processor
	wave.find_neighbour_for_other_processor();
    // building MPI_Datatypes for sending and receiving (calculate address displacements)
    wave.build_communication_type();

	
    // run simulation
	wave.run_simulation();

	if(!wave.display|| !wave.output)
	{
      if(id == 0)
	  {
		  	finish = chrono::high_resolution_clock::now();

	        elapsed = finish - start;
	        cout << setprecision(5);
	        cout << "The code took " << elapsed.count() << "s to run" << endl;
	  }
	}

    MPI_Finalize();

}