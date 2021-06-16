#pragma once
#include <iostream>
#include <vector>
#include "Matrix.h"
#include <memory>
#include "CSRMatrix.h"


template <class T>
class Solver
{
public:

    Solver();
    ~Solver();

     void dense_Jacobi(Matrix<T> &mat, T *RHS, T* solution, int iter_max = 1e3, double tol = 1e-6);

     void dense_Gauss_Seidel(Matrix<T>& mat, T* RHS, T* solution, int iter_max = 1e3, double tol = 1e-6);

     void dense_LU(Matrix<T>& mat, T* RHS, T* solution);

     void BiCGSTAB(Matrix<T>& mat, T* RHS, T* solution, int iter_max = 1e3, T tol = 1e-6);

     void sparse_Gauss_Seidel(CSRMatrix<T>& mat, T* RHS, T* solution, int iter_max = 1e3, T tol = 1e-6);

     void sparse_Jacobi(CSRMatrix<T>& mat, T* RHS, T* solution, int iter_max = 1e3, T tol = 1e-6);



};
