#include "Parallel.h"
#include <iostream>
#include <chrono>
#include <math.h>
#include <vector>
#include <memory>

using namespace std;


Parallel::Parallel(int size, int threads) {
    this->size = size;
    this->size_of_value = size * size;
    this->last_col_start = size * (size - 1);
    this->last = size - 1;
    this->threads = threads;
    this->diag = size + 1;

    this->value = new bool[size_of_value];
    this->neigh_value = new int[size_of_value];
    this->row_index = new int[size_of_value];
    this->col_index = new int[size_of_value];

}

Parallel::~Parallel() {
    delete[] this->value;
    delete[] this->neigh_value;
    delete[] this->col_index;
    delete[] this->row_index;
}

void Parallel::set_zero(int *int_pt) {
#pragma omp parallel num_threads(threads)
    {
#pragma omp for
        for (int i = 0; i < size_of_value; ++i) {
            int_pt[i] = 0;
        }

    }
}

void Parallel::get_index_row_col() {
#pragma omp parallel num_threads(threads)
    {
#pragma omp for
        for (int i = 0; i < size_of_value; ++i) {
            row_index[i] = i % size;
            col_index[i] = i / size;
        }
    }
}

void Parallel::get_index_row_col_serial() {
    for (int i = 0; i < size_of_value; ++i) {
        row_index[i] = i % size;
        col_index[i] = i / size;
    }
}

void Parallel::add_neighbours() {

#pragma omp parallel num_threads(threads) firstprivate(size, last, size_of_value, last_col_start, diag, row_index, col_index)
    {   
        #pragma omp for
        for (int id = 0; id < size_of_value; ++id) {
            if (value[id]) {

                    // adding left

                        if (col_index[id] != 0) {
#pragma omp atomic
                            ++neigh_value[id - size];

                        } else {
#pragma omp atomic
                            ++neigh_value[last_col_start + id];

                        }


                    // adding right

                        if (col_index[id] != last) {
#pragma omp atomic
                            ++neigh_value[id + size];
                        } else {
#pragma omp atomic
                            ++neigh_value[id - last_col_start];
                        }


                    // adding up

                        if (row_index[id] != 0) {
#pragma omp atomic
                            ++neigh_value[id - 1];
                        } else {
#pragma omp atomic
                            ++neigh_value[id + last];
                        }


                    // adding down

                        if (row_index[id] != last) {
#pragma omp atomic
                            ++neigh_value[id + 1];
                        } else {
#pragma omp atomic
                            ++neigh_value[id - last];
                        }



                    // adding up-left

                        if (row_index[id] != 0 && col_index[id] != 0) {
#pragma omp atomic
                            ++neigh_value[id - diag];
                        } else if (row_index[id] == 0 && col_index[id] != 0) {
#pragma omp atomic
                            ++neigh_value[id - 1];

                        } else if (row_index[id] != 0 && col_index[id] == 0) {
#pragma omp atomic
                            ++neigh_value[id + last_col_start - 1];
                        } else {
#pragma omp atomic
                            ++neigh_value[size_of_value - 1];
                        }




                    // adding up-right

                        if (row_index[id] != 0 && col_index[id] != last) {
#pragma omp atomic
                            ++neigh_value[id + last];
                        } else if (row_index[id] == 0 && col_index[id] != last) {
#pragma omp atomic
                            ++neigh_value[id + size + last];
                        } else if (row_index[id] != 0 && col_index[id] == last) {
#pragma omp atomic
                            ++neigh_value[id - last_col_start - 1];
                        } else {
#pragma omp atomic
                            ++neigh_value[last];
                        }




                    // adding down-left

                        if (row_index[id] != last && col_index[id] != 0) {
#pragma omp atomic
                            ++neigh_value[id - last];
                        } else if (row_index[id] == last && col_index[id] != 0) {
#pragma omp atomic
                            ++neigh_value[id - size - last];
                        } else if (row_index[id] != last && col_index[id] == 0) {
#pragma omp atomic
                            ++neigh_value[id + last_col_start + 1];
                        } else {
#pragma omp atomic
                            ++neigh_value[last_col_start];
                        }


                    // adding down-right

                        if (row_index[id] != last && col_index[id] != last) {
#pragma omp atomic
                            ++neigh_value[id + diag];
                        } else if (row_index[id] == last && col_index[id] != last) {
#pragma omp atomic
                            ++neigh_value[id + 1];
                        } else if (row_index[id] != last && col_index[id] == last) {
#pragma omp atomic
                            ++neigh_value[id - last_col_start + 1];
                        } else {
#pragma omp atomic
                            ++neigh_value[0];
                        }

            }
        }
    }

}

void Parallel::do_iteration_parallel() {
    add_neighbours();

#pragma omp parallel for num_threads(threads)
    for (int i = 0; i < size_of_value; ++i) {
        if (value[i]) {
            if (neigh_value[i] != 2 && neigh_value[i] != 3) {
                value[i] = false;
            }
        } else if (neigh_value[i] == 3) {
            value[i] = true;
        }

        neigh_value[i] = 0;
    }


}

void Parallel::error(vector<vector<bool>> grid, bool *value, int size) {
    int error = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            error += abs(grid[i][j] - value[i + j * size]);
        }
    }
    cout<< error;
}

void Parallel::convert_initial(vector<vector<bool>> grid, bool *value, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            value[i + j * size] = grid[i][j];
        }
    }

}

void Parallel::set_initial() {

#pragma omp parallel for num_threads(threads)
    for (int i = 0; i < size_of_value; ++i) {
        value[i] = (rand() % 2);
    }
}





