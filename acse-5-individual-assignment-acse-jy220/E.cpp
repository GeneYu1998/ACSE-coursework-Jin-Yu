#include <iostream>
#include <math.h>
#include <ctime>
#include "Matrix.h"
#include "Matrix.cpp"
#include "CSRMatrix.h"
#include "CSRMatrix.cpp"


int main()

{ int size = 3;
  
  auto value = new int[9] {1, 2, 3, 4, 5, 6, 7 ,8 ,9};
  auto matrix = new Matrix<int> (size, size, value);
  
  std::cout << "printing the matrix in row-major order: " << std::endl;
  matrix->printMatrix();

  auto vec = new int[3]{3, 4, 6};
  std::cout << "the vector we use to multiply: " << std::endl;
  // initialize a outcome vector
  auto vec_outcome = new int[3]{0, 0, 0};
  for (int i = 0; i < 3; i++)
  {
      std::cout << vec[i] << " ";
  }

  std::cout << std::endl;

  // creating a matrix copy
  auto matrix_cp1 = new Matrix<int>(*matrix);
  matrix_cp1->matVecMult_rowmajor(vec, vec_outcome);
  
  std::cout << "the result vector when the matrix is stored in row-major order: " << std::endl;
  for (int i = 0; i < 3; i++)
  {
      std::cout << vec_outcome[i] << " ";
  }
  
  std::cout << std::endl;

   // initialize a outcome vector
  auto vec_outcome2 = new int[3]{0, 0, 0};
  
  // creating a matrix copy
  auto matrix_cp2 = new Matrix<int>(*matrix);

  matrix_cp2->matVecMult_colmajor(vec, vec_outcome2);
  std::cout << "the result vector when the matrix is stored in column-major order: " << std::endl;
  for (int i = 0; i < 3; i++)
  {
      std::cout << vec_outcome2[i] << " ";
  }  
  std::cout << std::endl;  





  delete[] value;
  delete[] vec;
  delete[] vec_outcome;
}