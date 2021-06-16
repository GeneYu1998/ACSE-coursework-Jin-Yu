#pragma once
#include "Matrix.h"

template <class T>
class CSRMatrix: public Matrix<T>
{
public:

   // constructor where we want to preallocate ourselves
   CSRMatrix(int rows, int cols, int nnzs, bool preallocate);
   // constructor where we already have allocated memory outside
   CSRMatrix(int rows, int cols, int nnzs, T *values_ptr, int *row_position, int *col_index);
   // constructor that deep copys a existing CSRmatrix
   CSRMatrix(CSRMatrix<T>& input);
   // destructor
   ~CSRMatrix();

   // Print out the values in our matrix
	virtual void printMatrix();

   // generating random positive definite CSRMatrix values
   virtual void generate_positive_definite();

   // converting a dense matrix into CSR format
   void dense2sparse(Matrix<T>& input);

   // converting a sparse matrix into dense format
   Matrix<T>* sparse2dense();

   // Perform some operations with our matrix
   void matMatMult(CSRMatrix<T>& mat_left, CSRMatrix<T>& output);
   // Perform some operations with our matrix
   void matVecMult(T *input, T *output);   

   // Explicitly using the C++11 nullptr here
   int *row_position = nullptr;
   int *col_index = nullptr;

   // How many non-zero entries we have in the matrix
   int nnzs=-1;

// Private variables - there is no need for other classes 
// to know about these variables
private:
   
};