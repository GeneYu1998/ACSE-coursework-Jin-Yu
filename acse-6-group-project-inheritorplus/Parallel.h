#pragma once
#include <omp.h>
#include <vector>

using namespace std;


class Parallel {
public:
    Parallel(){};

    Parallel(int size, int threads);

    virtual ~Parallel();

    void set_zero(int *int_pt);

    void get_index_row_col();

    void get_index_row_col_serial();

    void add_neighbours();

    void do_iteration_parallel();

    void error(vector<vector<bool>> grid, bool *value, int size);

    void convert_initial(vector<vector<bool>> grid, bool *value, int size);

    void set_initial();

    int size, size_of_value, last_col_start, last, threads, diag;
    bool *value;
    int *neigh_value;
    int *row_index;
    int *col_index;



//private:
//    int size;

};


