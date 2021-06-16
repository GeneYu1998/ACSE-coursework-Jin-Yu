#include <iostream>
#include <math.h>
#include <ctime>
#include "Matrix.h"
#include "Matrix.cpp"
#include "CSRMatrix.h"
#include "CSRMatrix.cpp"


int main()

{ 
  int size = 5;
  int nnzs = 10;

  auto values = new int[nnzs] {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  auto row_position = new int[size + 1] {0, 3, 5 ,6, 8, 10};

  auto col_index = new int[nnzs]{0, 3, 4, 1, 3, 2, 1, 4, 0, 4};

  CSRMatrix<int>* sparse_example = new CSRMatrix<int>(size, size, nnzs, values, row_position, col_index);
  
  std::cout << "CSR format: " << std::endl;

  sparse_example->printMatrix();

  auto sptr = sparse_example->getDiagonal();

  for (int i = 0; i < sparse_example->rows; i++)
  {
    std::cout << (sptr.get())[i] << " ";
  }

  std::cout << std::endl;



  delete[] values;

  delete[] row_position;

  delete[] col_index;

}