#include "Solver.h"
#include <memory>
#include "cblas.h"

template <class T>
Solver<T>::Solver(){}

template <class T>
Solver<T>::~Solver(){}

// Applying Jacobi iteration method to a dense format Matrix.
template<class T>
void Solver<T>::dense_Jacobi(Matrix<T> &mat, T *RHS, T* solution, int iter_max, double tol)
{
    double diff;
    double norm = 1;
    int iter=0;
    int size = mat.rows;

    auto * x_k = new double [size] ;

    // An initial guess at the solution and x_k.
    for (int i = 0; i < size; i++)
    {
        solution[i] = RHS[i];
        x_k[i] = 0;
    }

    // Iterate up to max number of iterations, we will 'break'
    // this loop if we hit our convergence tolerance
    while(tol * tol <= norm && iter < iter_max){
        norm = 0;
        iter++;
        for (int i = 0; i < size; i++)
        {
            double sum = 0; // to store the sum of (a_ij * x_j)
            for (int j = 0; j < size; j++)
            {
                if (i != j) {
                    sum += mat.values[i + j * size] * x_k[j];
                }
            }
            solution[i] =
                    (1. / mat.values[i + i * size]) *
                    (RHS[i] - sum);

            diff = solution[i] - x_k[i]; // calculate the difference
            norm += diff * diff; // calculate the norm

        }

        // assign x_k+1 (solution[i]) to x_k then do next loop
        for (int i = 0; i < size; i++)
        {
            x_k[i] = solution[i];
        }
    }

    delete[] x_k;
    std::cout << "total number of iterations used in Jacobi:  " << iter << std::endl;

}

// Applying Gauss Seidel iteration method to a dense format Matrix.
template <class T>
void Solver<T>::dense_Gauss_Seidel(Matrix<T>& mat, T* RHS, T* solution, int iter_max, double tol)
{
    int size = mat.rows;
    double factor;
    double diff;
    double thisstep;
    double norm = 1;
    int iter = 0;

    for (int i = 0; i < size; i++)
    {
        solution[i] = RHS[i];
    }

    while(tol * tol <= norm && iter < iter_max)
    { norm = 0;
        iter++;
        for (int i = 0; i < size; i++)
        {
            factor = 1 / mat.values[i + i * size];
            thisstep = RHS[i];

            for (int j = 0; j < size; j++)
            {
                if (j != i)
                {
                    thisstep -= mat.values[i + j * size] * solution[j];
                }
            }

            thisstep *= factor;
            diff = solution[i] - thisstep;
            norm += diff * diff;
            solution[i] = thisstep;
        }
    }
    std::cout << "total number of iterations used in Gauss_Seidel:  " << iter << std::endl;
}

// Applying LU factorisation method to a dense format Matrix.
template<class T>
void Solver<T>::dense_LU(Matrix<T> &mat, T *RHS, T *solution) {
    int rows = mat.rows;
    int cols = mat.cols;

    auto *matA = new Matrix<double>(mat); // for operation of matrix L
    auto *L = new Matrix<double>(rows, cols, true); // for operation of matrix L
    auto *U = new Matrix<double>(rows, cols, true); // to store the result of matrix U

    // do LU_decomposition for matrix A
    for (int k = 0; k < cols-1; k++) {
        for (int i = k+1; i < cols; i++) {
            double s = matA->values[i + k * cols] / matA->values[k + k * cols];
            for (int j = k; j < cols; j++) {
                matA->values[i + j * cols] = matA->values[i + j * cols] - s * matA->values[k + j * cols];
            }
            L->values[i +k * cols] = s;
        }
    }

    //add in the ones on the main diagonal to L
    for (int i = 0; i < rows; i++) {
        L->values[i + i * rows] += 1;
    }

    // assign the values to matrix U
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            U->values[i +j * cols] = matA->values[i + j * cols] ;
        }
    }

    // forward_substitution
    auto *y = new double [cols];
    for (int k = 0; k < cols; k++) {
        double s = 0.;
        for (int j = 0; j < k; j++) {
            s = s + L->values[k + j * cols] * y[j];
        }
        y[k] = (RHS[k] - s) / L->values[k + k * cols];
    }


    //backward_substitution
    int n = cols;
    for (int k = n-1; k >-1 ; k--) {
        double s = 0;
        for (int j = k+1; j < n; j++) {
            s = s + U->values[k + j * n] * solution[j];
        }
        solution[k] = (y[k] - s) / U->values[k + k * n];
    }

    delete L;
    delete U;
    delete[] y;
    delete matA;
}

//an Unpreconditioned BiCGSTAB with the initial guass x0 is zero vector.
template <class T>
void Solver<T>::BiCGSTAB(Matrix<T>& mat, T* RHS, T* solution, int iter_max, T tol)
{
     int iter;
     int size = mat.rows;
     T rho, rho_this, alpha, beta, omega, err, rho_next;
     std::shared_ptr<T> r_sptr(new T[size]);
     std::shared_ptr<T> r_temp_sptr(new T[size]);
     std::shared_ptr<T> v_sptr(new T[size]);
     std::shared_ptr<T> p_sptr(new T[size]);
     std::shared_ptr<T> rh_sptr(new T[size]);
     std::shared_ptr<T> h_sptr(new T[size]);
     std::shared_ptr<T> s_sptr(new T[size]);
     std::shared_ptr<T> t_sptr(new T[size]);
     std::shared_ptr<T> res_sptr(new T[size]);
     T *r = r_sptr.get();
     T *rt = r_sptr.get();
     T *v = v_sptr.get();
     T *p = p_sptr.get();
     T *rh = rh_sptr.get();
     T *h = h_sptr.get();
     T *s = s_sptr.get();
     T *t = t_sptr.get();
     T *res = res_sptr.get();

    // initial gauss be 0
     for (int i = 0; i < size; i++)
     {
        solution[i] = 0;
     }

    // initialize r, v, p, rh
     cblas_dcopy(size, RHS, 1, r, 1);
     cblas_dcopy(size, RHS, 1, rh, 1);
     cblas_dcopy(size, solution, 1, p, 1);
     cblas_dcopy(size, solution, 1, v, 1);

     rho = 1;
     alpha = 1;
     omega = 1;

     while (iter < iter_max)
     {  iter++;
        rho_this = rho;
        rho = cblas_ddot(size, rh, 1, r, 1);
        beta = (rho/rho_this) * (alpha/omega);
        cblas_daxpy(size, -omega, v, 1, p, 1);
        cblas_dcopy(size, r, 1, rt, 1);
        cblas_daxpy(size, beta, p, 1, rt, 1);
        cblas_dcopy(size, rt, 1, p, 1);
        v = mat.matVecMult(p);
        alpha = rho_next;
        alpha /= cblas_ddot(size, rh, 1, v, 1);
        cblas_dcopy(size, solution, 1, h, 1);
        cblas_daxpy(size, alpha, p, 1, h, 1);

        res = mat.matVecMult(h);
        cblas_daxpy(size, -1, RHS, 1, res, 1);
        err = cblas_dnrm2(size, res, 1);
        if (tol >= err)
        {
        cblas_dcopy(size, h, 1, solution, 1);
        std::cout << "total iterations used for BiCGSTAB: " << iter << std::endl;
        break;
        }

        cblas_dcopy(size, r, 1, s, 1);
        cblas_daxpy(size, -alpha, v, 1, s, 1);
        t = mat.matVecMult(s);
        omega = cblas_ddot(size, t, 1, s, 1) / cblas_ddot(size, t, 1, t, 1);
        cblas_dcopy(size, h, 1, solution, 1);
        cblas_daxpy(size, omega, s, 1, solution, 1);
        res = mat.matVecMult(solution);
        cblas_daxpy(size, -1, RHS, 1, res, 1);
        err = cblas_dnrm2(size, res, 1);
        if (tol >= err)
        { std::cout << "total iterations used for BiCGSTAB: " << iter << std::endl;
            break;}
        cblas_dcopy(size, s, 1, r, 1);
        cblas_daxpy(size, -omega, t, 1, r, 1);
     }

}

// Applying Gauss_Seidel iteration method to a CSR format Matrix.
template <class T>
void Solver<T>::sparse_Gauss_Seidel(CSRMatrix<T>& mat, T* RHS, T* solution, int iter_max, T tol)
{   
    int size = mat.rows;
    T factor;
    T diff;
    T thisstep;
    T norm = 1;
    int iter = 0;
    int ccol;
    
    // Set the initial gauss to be b.
    cblas_dcopy(size, RHS, 1, solution, 1);

    while(tol * tol <= norm)
    { norm = 0;
      iter++;
    for (int i = 0; i < size; i++)
    {   
        thisstep = RHS[i];

        for (int j = mat.row_position[i]; j < mat.row_position[i+1]; j++)
        {
            ccol = mat.col_index[j];
            if (ccol != i)
            {
            thisstep -= mat.values[j] * solution[ccol];
            }
            else if(ccol == i)  
            {
                factor = T(1 / mat.values[j]);
            }

        }

        thisstep *= factor;
        diff = solution[i] - thisstep;
        norm += diff * diff;
        solution[i] = thisstep; 
    }
    }


    std::cout << "Total number of iterations sparse_Gauss_Seidel used:  " << iter << std::endl;
}

// Applying Jacobi iteration method to a CSR format Matrix.
template <class T>
void Solver<T>::sparse_Jacobi(CSRMatrix<T>& mat, T* RHS, T* solution, int iter_max, T tol)
{   int size = mat.rows;
    T factor;
    T diff;
    T thisstep;
    T norm = 1;
    int iter = 0;
    int ccol;
    
    // using a shared pointer to store the x for each iteration
    std::shared_ptr<T> temp_sptr(new T[size]);
    auto temp = temp_sptr.get();
    
    // Set the initial gauss to be b.
    cblas_dcopy(size, RHS, 1, solution, 1);

    while(tol * tol <= norm)
    { norm = 0;
      iter++;
      // storing the x from last iteration
      cblas_dcopy(size, solution, 1, temp, 1);

    for (int i = 0; i < size; i++)
    {   
        thisstep = RHS[i];

        for (int j = mat.row_position[i]; j < mat.row_position[i+1]; j++)
        {
            ccol = mat.col_index[j];
            if (ccol != i)
            {
            thisstep -= mat.values[j] * temp[ccol];
            }
            else if(ccol == i)  
            {
                factor = T(1 / mat.values[j]);
            }

        }

        thisstep *= factor;
        solution[i] = thisstep;
        diff = solution[i] - temp[i];
        norm += diff * diff; 
    }


    }


    std::cout << "Total number of iterations sparse_Jacobi used:  " << iter << std::endl;

}


