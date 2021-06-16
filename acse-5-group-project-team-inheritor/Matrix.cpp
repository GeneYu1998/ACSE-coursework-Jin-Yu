#include <iostream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <iomanip>
#include <stdlib.h>
#include "Matrix.h"
#include <memory>
#include <vector>

// Constructor - using an initialisation list here
template<class T>
Matrix<T>::Matrix(int rows, int cols, bool preallocate): rows(rows), cols(cols), size_of_values(rows * cols),
                                                         preallocated(preallocate) {
    // If we want to handle memory ourselves
    if (this->preallocated) {
        // Must remember to delete this in the destructor
        this->values = new T[size_of_values];
    }
}

// Constructor - now just setting the value of our T pointer
template<class T>
Matrix<T>::Matrix(int rows, int cols, T *values_ptr): rows(rows), cols(cols), size_of_values(rows * cols),
                                                      values(values_ptr) {}

// The copy constructor for Matrix<T>
template<class T>
Matrix<T>::Matrix(Matrix<T> &input): rows(input.rows), cols(input.cols), size_of_values(input.rows * input.cols) {
    this->preallocated = true;
    this->values = new T[this->size_of_values];
    for (int i = 0; i < this->size_of_values; i++) {
        this->values[i] = input.values[i];
    }
}

// destructor
template<class T>
Matrix<T>::~Matrix() {
    // Delete the values array
    if (this->preallocated) {
        delete[] this->values;
    }
}

// Just print out the values in our values array
template<class T>
void Matrix<T>::printValues() {
    std::cout << "Printing values" << std::endl;
    for (int i = 0; i < this->size_of_values; i++) {
        std::cout << this->values[i] << " ";
    }
    std::cout << std::endl;
}

// Explicitly print out the values in values array as if they are a matrix
template<class T>
void Matrix<T>::printMatrix() {
    std::cout << "Printing matrix" << std::endl;
    for (int j = 0; j < this->rows; j++) {
        std::cout << std::endl;
        for (int i = 0; i < this->cols; i++) {
            // There is an column-major ordering assumption here
            std::cout << this->values[j + i * this->cols] << " ";
        }
    }
    std::cout << std::endl;
}

// Do matrix matrix multiplication
// There is an column - major ordering assumption in this function
// output = this * mat_right
template<class T>
void Matrix<T>::matMatMult(Matrix<T> &mat_right, Matrix<T> &output) {
    int row = this->rows;
    int col = mat_right.cols;
    int right_row = mat_right.rows;

    if (output.values == nullptr) {
        output.values = new T[row * col];
        output.preallocated = true;
    }

    // error checking for sizes!!
    if (this->cols != mat_right.rows) {
        std::cerr << "Matrix size error: the number of columns of the left matrix\
        does not match the rows of the right matrix." << std::endl;
    }

    // set to zero
    for (int i = 0; i < output.size_of_values; i++) {
        output.values[i] = 0.0;
    }

    int l = 0;
    int r = 0;
    int id;
    T mrv;

    // do the multiplication
    for (int j = 0; j < col; j++) {
        l = j * row;
        r = j * mat_right.rows;
        id = 0;

        for (int k = 0; k < right_row; k++) {
            mrv = mat_right.values[r];

            for (int i = 0; i < row; i++) {
                output.values[l] += this->values[id] * mrv;

                l++;
                id++;
            }

            r++;
            l -= row;
        }
    }
}


// Do matrix vector multiplication and return the output.
// There is an column - major ordering assumption in this function
// output = this * vec
template<class T>
T *Matrix<T>::matVecMult(T *vec) {
    int size = this->rows;
    auto output = new T[size];


    // set the output vector to zero
    for (int i = 0; i < size; i++) {
        output[i] = 0.0;
    }

    int r = 0;
    int l = 0;
    int id = 0;
    T mrv;

    // do the multiplication

    for (int k = 0; k < size; k++) {
        mrv = vec[r];

        for (int i = 0; i < size; i++) {
            output[l] += this->values[id] * mrv;

            l++;
            id++;
        }

        r++;
        l -= size;
    }

    return output;
}


// generating a random positive definite matrix values (column-major ordering)
template<class T>
void Matrix<T>::generate_positive_definite() {
    int size = this->size_of_values;
    int row = this->rows;
    T diag;
    std::shared_ptr<T> up_sptr(new T[size]);
    std::shared_ptr<T> lowr_sptr(new T[size]);
    T *upper_triangle = up_sptr.get();
    T *lower_triangle = lowr_sptr.get();

    for (int i = 0; i < size; i += row) {  // diagonal entry from 50 to 100.
        diag = T(rand() % 50 + 50);
        upper_triangle[i] = diag;
        lower_triangle[i] = diag;
        ++i;
    }

    // non-diagonal entry in [-5, 5].
    for (int j = 1; j < row; j++) {
        for (int i = 0; i < j; i++) {
            upper_triangle[i + j * row] = T(rand() % 10 - 5);
        }
    }

    for (int j = 0; j < row; j++) {
        for (int i = j + 1; i < row; i++) {
            lower_triangle[i + j * row] = T(rand() % 10 - 5);
        }
    }

    auto mat_left = new Matrix<T>(rows, cols, upper_triangle);
    auto mat_right = new Matrix<T>(rows, cols, lower_triangle);

    mat_left->matMatMult(*mat_right, *this);
}

