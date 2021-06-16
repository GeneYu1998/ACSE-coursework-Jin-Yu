#pragma once

template <class T>
class Matrix
{
public:

   // constructor where we want to preallocate ourselves
   Matrix(int rows, int cols, bool preallocate);
   // constructor where we already have allocated memory outside
   Matrix(int rows, int cols, T *values_ptr);
   // copy constrcutor
   Matrix(Matrix<T>& input);
   // destructor
   virtual ~Matrix();

   // Print out the values in our matrix
   void printValues();
	virtual void printMatrix();
   
   // matVecMult functions
   void matVecMult_colmajor(T *input, T *output);
   void matVecMult_rowmajor(T *input, T *output);

   // Perform some operations with our matrix
   void matMatMult(Matrix<T>& mat_left, Matrix<T>& output);

   // Explicitly using the C++11 nullptr here
   T *values = nullptr;   
   int rows = -1;
   int cols = -1;

// We want our subclass to know about this
protected:
   bool preallocated = false;

// Private variables - there is no need for other classes 
// to know about these variables
private:

   int size_of_values = -1;   
};