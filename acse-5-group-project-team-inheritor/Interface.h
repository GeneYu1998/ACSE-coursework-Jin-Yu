#pragma once

#include <string>
#include <iostream>
#include "Matrix.h"
#include "CSRMatrix.h"



class Interface {
public:
    Interface();

    virtual ~Interface();


private:

    void interfaceIntro();

    void selectFormat();

    // interface for dense
    //----------------------------- DENSE ----------------------------------------

    void initDense();

    static int read_row(const char* filename);

    static std::vector<std::vector<double>>  readMatrixFile(int size, const char *fileName);

    template<class T>
    void select_denseSolver(Matrix<T> &mat, T *RHS, T* solution, int size);

    template <class T>
    void print_vector (T *vec, int size);

    template<class T>
    T * generate_random_vector(T *vec, int size);

    void denseCreate();

    template<class T>
    void solver_dense_Jacobi(Matrix<T> &mat, T *RHS, T *solution, int size);

    template<class T>
    T compute_error(T *vec, T *vec2, int size);

    template<class T>
    void solver_dense_GaussSeidal(Matrix<T> &mat, T *RHS, T *solution, int size);

    template<class T>
    void solver_dense_LU(Matrix<T> &mat, T *RHS, T *solution, int size);

    template<class T>
    void solver_BiCGSTAB(Matrix<T> &mat, T *RHS, T *solution, int size);

    template<class T>
    void solver_all(Matrix<T> &mat, T *RHS, int size);

    void loadDenseFirst();

    void loadDense();


    //----------------------------- DENSE ----------------------------------------


    // Interface for CSR
    //----------------------------- CSR ------------------------------------------

    void initCSR();

    void csr_create();

    void loadCSRFirst();

    void loadCSR();

    template<class T>
    void select_sparseSolver(CSRMatrix<T>& mat, T* RHS, T* solution, int size);

    template<class T>
    void solver_sparse_GaussSeidel(CSRMatrix<T> &mat, T *RHS, T* solution, int size);

    template<class T>
    void solver_sparse_Jacobi(CSRMatrix<T> &mat, T *RHS, T* solution, int size);


    template<class T>
    void solver_sparse_all(CSRMatrix<T> &mat, T *RHS, int size);

    static std::vector<std::vector<double> > read_csr(std::string pathname);

    //----------------------------- CSR ------------------------------------------

    void invalid(std::string message);

};


