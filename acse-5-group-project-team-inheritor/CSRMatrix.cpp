#include <iostream>
#include "CSRMatrix.h"
#include <vector>
#include <memory>

// Constructor - using an initialisation list here
template <class T>
CSRMatrix<T>::CSRMatrix(int rows, int cols, int nnzs, bool preallocate): Matrix<T>(rows, cols, false), nnzs(nnzs)
{
   // If we don't pass false in the initialisation list base constructor, it would allocate values to be of size
   // rows * cols in our base matrix class
   // So then we need to set it to the real value we had passed in
   this->preallocated = preallocate;

   // If we want to handle memory ourselves
   if (this->preallocated)
   {
      // Must remember to delete this in the destructor
      this->values = new T[this->nnzs];
      this->row_position = new int[this->rows+1];
      this->col_index = new int[this->nnzs];
   }
}

// Constructor - now just setting the value of our T pointer
template <class T>
CSRMatrix<T>::CSRMatrix(int rows, int cols, int nnzs, T *values_ptr, int *row_position, int *col_index): Matrix<T>(rows, cols, values_ptr), nnzs(nnzs), row_position(row_position), col_index(col_index)
{}

// Constructor which creates a deep copy
template <class T>
CSRMatrix<T>::CSRMatrix(CSRMatrix<T>& input): Matrix<T>(input)
{  
   this->preallocated =true;
   this->nnzs = input.nnzs;
   // Must remember to delete this in the destructor
   this->values = new T[input.nnzs];
   this->row_position = new int[input.rows+1];
   this->col_index = new int[input.nnzs];

   for (int i = 0; i < nnzs; i++)
   {
      this->values[i] = input.values[i];
      this->col_index[i] = input.col_index[i];
   }

   for (int j = 0; j <= input.rows; j++)
   {
      this->row_position[j] = input.row_position[j];
   }

}

// destructor
template <class T>
CSRMatrix<T>::~CSRMatrix()
{
   // Delete the values array
   if (this->preallocated){
      delete[] this->row_position;
      delete[] this->col_index;
   }
   // The super destructor is called after we finish here
   // This will delete this->values if preallocated is true
}

// Explicitly print out the values in values array as if they are a matrix
template <class T>
void CSRMatrix<T>::printMatrix() 
{ 
   std::cout << "Printing matrix" << std::endl;
   std::cout << "Values: ";
   for (int j = 0; j< this->nnzs; j++)
   {  
      std::cout << this->values[j] << " ";      
   }
   std::cout << std::endl;
   std::cout << "row_position: ";
   for (int j = 0; j< this->rows+1; j++)
   {  
      std::cout << this->row_position[j] << " ";      
   }
   std::cout << std::endl;   
   std::cout << "col_index: ";
   for (int j = 0; j< this->nnzs; j++)
   {  
      std::cout << this->col_index[j] << " ";      
   }
   std::cout << std::endl;   
}


// converting CSR matrix to a dense matrix (column-major ordering)
template <class T>
Matrix<T>* CSRMatrix<T>::sparse2dense()
{   
   auto dense = new Matrix<T>(this->rows, this->cols, true);

    for (int i = 0; i < dense->rows * dense->cols; i++)
    {
       dense->values[i] = 0;
    }

    for (int i = 0; i < this->rows; i++)
    {
       for (int j = this->row_position[i]; j < this->row_position[i+1]; j++)
       {
          dense->values[i + (this->col_index)[j] * dense->rows] = (this->values)[j];       
          }
    }

    return dense;
}


// converting dense Matrix to a CSRMatrix
template <class T>
void CSRMatrix<T>::dense2sparse(Matrix<T>& input)
{  
   if(this->preallocated)
   {
      std::cerr << "this CSRMatrix should not be preallocated!" << std::endl;      
   }

   this->preallocated = true;

   int size = input.rows;
   
   this->row_position = new int[size + 1];
   std::vector<int> column_index;
   std::vector<T> value_vector;
   int number_of_nnzs = 0;
   T cv;
   
   this->row_position[0] = 0;

   for (int i = 0; i < size; i++)
   {  
      for (int j = 0; j < size; j++)
      {
         cv = input.values[i + j * size];
         if(cv != 0)
         {
            number_of_nnzs++;
            value_vector.push_back(cv);
            column_index.push_back(j);
         }
      }
      this->row_position[i + 1] = number_of_nnzs;
   }
   
   this->nnzs = number_of_nnzs;
   this->values = new T[number_of_nnzs];
   this->col_index = new int[number_of_nnzs];

   // assign the values of vectors back to arrays.
   for (int i = 0; i < number_of_nnzs; i++)
   {
       this->values[i] = value_vector[i];
       this->col_index[i] = column_index[i];
   }   

}


// Do a matrix-vector product
// output = this * input
template<class T>
void CSRMatrix<T>::matVecMult(T *input, T *output)
{
   if (input == nullptr || output == nullptr)
   {
      std::cerr << "Input or output haven't been created" << std::endl;
      return;
   }

   // Set the output to zero
   for (int i = 0; i < this->rows; i++)
   {
      output[i] = 0.0;
   }

   // Loop over each row
   for (int i = 0; i < this->rows; i++)
   {
      // Loop over all the entries in this col
      for (int val_index = this->row_position[i]; val_index < this->row_position[i+1]; val_index++)
      {
         // This is an example of indirect addressing
         // Can make it harder for the compiler to vectorise!
         output[i] += this->values[val_index] * input[this->col_index[val_index]];

      }
   }
}


// Do matrix matrix multiplication in CSRformat
// output = this * mat_right
template <class T>
void CSRMatrix<T>::matMatMult(CSRMatrix<T>& mat_right, CSRMatrix<T>& output)
{
   // Check our dimensions match
   if (this->cols != output.cols)
   {
      std::cerr << "Input dimensions for matrices don't match" << std::endl;
      return;
   }

   // Check if our output matrix has had space allocated to it
   if (output.values != nullptr) 
   {
      // Check our dimensions match
      if (this->rows != mat_right.cols || mat_right.rows != output.rows)
      {
         std::cerr << "Input dimensions for matrices don't match" << std::endl;
         return;
      }
      std::cerr << "output should not be preallocated!" << std::endl;      
   }
   
   // use <vector> container, to enable change of sizes.
   std::vector<T> value;
   output.row_position = new int[this->rows + 1];
   std::shared_ptr<T> sptr_temp(new T[mat_right.cols]);
   T* temp = sptr_temp.get();
   std::vector<int> column_index;
   int number_of_nnzs = 0;
   T lv;
   int rri, col_index_right;

   for (int i = 0; i < mat_right.cols; i++)
   {
      temp[i] = 0;
   }

   for (int i = 0; i < this->rows; i++)
   {  
      output.row_position[i] = number_of_nnzs;
      for(int k = this->row_position[i]; k < this->row_position[i + 1]; k++)
      {
          lv = this->values[k];
          rri = this->col_index[k];
          for (int j = mat_right.row_position[rri]; j < mat_right.row_position[rri + 1]; j++)
          {
              col_index_right = mat_right.col_index[j];
              temp[col_index_right] += lv * mat_right.values[j];

          }
      }

      for (int p = 0; p < mat_right.cols; p++)
      {  
         if(temp[p]!= 0)
         {
            value.push_back(temp[p]);
            column_index.push_back(p);
            number_of_nnzs++;
            temp[p] = 0;            
         }

      }
   }
   
   output.row_position[this->rows] = number_of_nnzs;
   output.nnzs = number_of_nnzs;
   
   output.values = new T[number_of_nnzs];
   output.col_index = new int[number_of_nnzs];
   output.preallocated = true;
   
   // assign the values in the vector containers back to arrays.
   for (int i = 0; i < number_of_nnzs; i++)
   {
       output.values[i] = value[i];
       output.col_index[i] = column_index[i];
   }

}


// generating random positive definite CSRMatrix values
template <class T>
void CSRMatrix<T>::generate_positive_definite()
{  
     int size = this->rows;
     int sq = size * size;
     int nnzs_temp = sq;
     int fill = rand()%size + size;
     T diag;
     std::shared_ptr<T> up_sptr(new T[sq]);
     std::shared_ptr<T> lowr_sptr(new T[sq]);
     std::shared_ptr<CSRMatrix<T>> upper_sparse_sptr(new CSRMatrix<T>(size, size, 0, false));
     std::shared_ptr<CSRMatrix<T>> lower_sparse_sptr(new CSRMatrix<T>(size, size, 0, false));
     T *upper_triangle = up_sptr.get();
     T *lower_triangle = lowr_sptr.get();
     CSRMatrix<T>* upper_sparse = upper_sparse_sptr.get();
     CSRMatrix<T>* lower_sparse = lower_sparse_sptr.get();
     int random, m, r;

      for (int i = 0; i < sq; i++)
       {
          upper_triangle[i] = 0;
          lower_triangle[i] = 0;
       }

      for(int i = 0; i < sq; i += size)
      {  
      // diagonal entry from 50 to 100.
      diag = T(rand() % 50 + 50);
      upper_triangle[i] = diag;
      lower_triangle[i] = diag;
      ++i;
      }
      
      //upper triangle
      for (int i = 0; i < fill; ++i)
      {   
       random = rand()%sq;
            r = random % size;
            m = (random - r) / size;
       if(m <= r)
       {--i;}
       else
       {
            upper_triangle[random] = T(rand()%10 - 5);
       }
       
       }
   
      //lower triangle
      for (int i = 0; i < fill; ++i)
      {   
       random = rand()%sq;
            r = random % size;
            m = (random - r) / size;
       if(m >= r)
       {--i;}
       else
       {
            lower_triangle[random] = T(rand()%10 - 5);
       }
       
       }
       
       auto upper_dense = new Matrix<T>(size, size, upper_triangle);
       auto lower_dense = new Matrix<T>(size, size, lower_triangle);
       
       //converting upper and lower tri. to CSR format
       upper_sparse->dense2sparse(*upper_dense);
       lower_sparse->dense2sparse(*lower_dense);

       // perform sparse multilications
       upper_sparse->matMatMult(*lower_sparse, *this);

}

