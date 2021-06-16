#include "Interface.h"
#include "Matrix.h"
#include "Matrix.cpp"
#include <iostream>
#include <ctime>
#include <string>
#include <fstream>
#include <sstream>
#include "Solver.h"
#include "Solver.cpp"
#include "CSRMatrix.h"
#include "CSRMatrix.cpp"

using namespace std;


Interface::Interface() {
    interfaceIntro();
}

Interface::~Interface() {}

void Interface::interfaceIntro() {
    cout << "***********************************************" << endl;
    cout << "-----------------------------------------------" << endl;
    cout << " ACSE-5: Advanced Programming Assignment" << endl;
    cout << "-----------------------------------------------" << endl;
    cout << endl;
    cout << " Yuchen Wang" << endl << " Jin Yu" << endl << " Xuyuan Chen" << endl;
    cout << endl;
    cout << "-----------------------------------------------" << endl;
    cout << endl;
    cout << "This software is designed for implement an algorithm to solve the linear system Ax = b, \n"
            "where A is a positive definite matrix, and x and b" << endl;
    cout << endl;
    cout << "-----------------------------------------------" << endl;
    cout << " Would you like to continue? (y/n)" << endl;
    cout << " >> ";
    char select;
    cin >> select;

    switch (select) {
        case 'y':
            break;
        case 'n':
            exit(0);
        default:
            interfaceIntro();
    }

    cout << endl;
    selectFormat();
}


void Interface::selectFormat() {
    cout << "***********************************************" << endl;
    cout << "Please select a format of the matrix to solve " << endl;
    cout << endl;
    cout << " 1. dense" << endl;
    cout << " 2. sparse" << endl;
    cout << " b: Back" << endl;
    cout << " x: Exit" << endl << endl;
    cout << " >> ";
    string select;
    cin >> select;
    cout << "***********************************************" << endl;

    // string to char
    char *select_char = new char[1];
    if (select.size() != 1) { *select_char = '#'; }
    else { copy(select.begin(), select.end(), select_char); }

    switch (*select_char) {
        case '1':
            initDense();
            break;
        case '2':
            initCSR();
            break;
        case 'b':
            cin.ignore();
            interfaceIntro();
            break;
        case 'x':
            exit(0);
        default:
            invalid("invalid, please enter again");
            selectFormat();
    }
    delete[] select_char;
}

// ------------------------------------ DENSE  ----------------------------------------

void Interface::initDense() {
    cout << "***********************************************" << endl;
    cout << "Select operation for matrix A and vector b " << endl;
    cout << endl;
    cout << " 1. Randomly Create a test example" << endl;
    cout << " 2. Load a dense matrix and a vector by .txt" << endl;
    cout << " b: Back" << endl;
    cout << " x: Exit" << endl << endl;
    cout << " >> ";
    string select;
    cin >> select;
    cout << "***********************************************" << endl;

    // string to char
    char *select_char = new char[1];
    if (select.size() != 1) { *select_char = '#'; }
    else { copy(select.begin(), select.end(), select_char); }

    switch (*select_char) {
        case '1':
            denseCreate();
            break;
        case '2':
            loadDenseFirst();
            break;
        case 'b':
            cin.ignore();
            selectFormat();
            break;
        case 'x':
            exit(0);
        default:
            invalid("please enter again");
            initDense();
    }
    delete[] select_char;
}

void Interface::denseCreate() {
    cout << endl;
    cout << "Please enter the size of matrix A: ";
    int input_size = 0;
    cin >> input_size;
    int size = input_size;
    if (cin.fail()) {
        invalid(" Please enter only integer");
        denseCreate();
    }

    auto *mat_experi = new Matrix<double>(size, size, true);

    clock_t start = clock();
    mat_experi->generate_positive_definite();
    clock_t end = clock();

    cout << "positive definite dense mat: " << endl;
    mat_experi->printMatrix();
    cout << "Time spent for generating matrix: " << (double) (end - start) / CLOCKS_PER_SEC * 1000.0 << endl;

    auto b = new double[size];

    generate_random_vector(b, size);
    cout << "The RHS vector generated: " << endl;
    print_vector(b, size);

    auto x = new double[size];

    for (int i = 0; i < size; i++) {
        x[i] = 0;
    }

    select_denseSolver(*mat_experi, b, x, size);

    delete[] x;
    delete[] b;
}

template<class T>
void Interface::select_denseSolver(Matrix<T> &mat_experi, T *b, T *x, int size) {
    cout << "***********************************************" << endl;
    cout << "There have 4 solvers for dense in this program " << endl;
    cout << " 1. dense_Jacobi " << endl;
    cout << " 2. dense_Gauss Seidel " << endl;
    cout << " 3. dense_LU factorization" << endl;
    cout << " 4. Biconjugate gradient stabilized method" << endl;
    cout << " a. compare 4 methods and get the feedback" << endl;
    cout << " x: Exit" << endl << endl;
    cout << "Please choose a method or enter 'a' to run all solvers" << endl;
    cout << " >> ";
    string select;
    cin >> select;
    cout << "***********************************************" << endl;
    // string to char
    char *select_char = new char[1];
    if (select.size() != 1) { *select_char = '#'; }
    else { copy(select.begin(), select.end(), select_char); }

    switch (*select_char) {
        case '1':
            solver_dense_Jacobi(mat_experi, b, x, size);
            break;
        case '2':
            solver_dense_GaussSeidal(mat_experi, b, x, size);
            break;
        case '3':
            solver_dense_LU(mat_experi, b, x, size);
            break;
        case '4':
            solver_BiCGSTAB(mat_experi, b, x, size);
            break;
        case 'a':
            solver_all(mat_experi, b, size);
            break;
        case 'x':
            exit(0);
        default:
            invalid("please enter again");
            initDense();
    }
    delete[] select_char;
}

// evaluate the L2-error
template<class T>
T Interface::compute_error(T *vec, T *vec2, int size) {
    T err = 0;
    T res;

    for (int i = 0; i < size; i++) {
        res = vec[i] - vec2[i];
        err += res * res;
    }

    cout << endl;

    return err;
}

template<class T>
void Interface::solver_dense_Jacobi(Matrix<T> &mat, T *RHS, T *solution, int size) {
    auto solver = new Solver<double>();

    clock_t start = clock();
    solver->dense_Jacobi(mat, RHS, solution);
    clock_t end = clock();
    cout << "------------------------------------------------: " << endl << endl;
    cout << "Results for Jacobi: " << endl;
    cout << "------------------: " << endl << endl;
    cout << "time spent(Jacobi): " << (double) (end - start) / CLOCKS_PER_SEC * 1000.0 << endl;
    cout << "x: " << endl;
    print_vector(solution, size);
    cout << "back subsititution : " << endl;
    auto outvec = mat.matVecMult(solution);
    print_vector(outvec, size);

    cout << "b: " << endl;
    print_vector(RHS, size);

    auto err = compute_error(RHS, outvec, size);
    cout << "absolute L2 error for dense_Jacobi: " << err << endl << endl;

    cout << "***********************************************" << endl;
    cout << "Choose the next step: " << endl;
    cout << endl;
    cout << " 1. back to the page of create/load dense matrix " << endl;
    cout << " 2. back to the page of choosing dense/sparse" << endl;
    cout << " x: Exit" << endl << endl;
    cout << " >> ";
    string select;
    cin >> select;
    cout << "***********************************************" << endl;

    // string to char
    char *select_char = new char[1];
    if (select.size() != 1) { *select_char = '#'; }
    else { copy(select.begin(), select.end(), select_char); }

    switch (*select_char) {
        case '1':
            initDense();
            break;
        case '2':
            selectFormat();
            break;
        case 'x':
            exit(0);
        default:
            invalid("invalid, please enter again");
            selectFormat();
    }

    delete[] select_char;
    delete[] outvec;
    delete[] solution;
    delete[] RHS;
}

template<class T>
void Interface::solver_dense_GaussSeidal(Matrix<T> &mat, T *RHS, T *solution, int size) {
    auto solver = new Solver<double>();

    clock_t start = clock();
    solver->dense_Gauss_Seidel(mat, RHS, solution);
    clock_t end = clock();
    cout << "------------------------------------------------: " << endl << endl;
    cout << "Results for Gauss-Seidal: " << endl;
    cout << "--------------------------: " << endl << endl;
    cout << "time spent(GaussSeidal): " << (double) (end - start) / CLOCKS_PER_SEC * 1000.0 << endl;
    cout << "x: " << endl;
    print_vector(solution, size);
    cout << "back subsititution : " << endl;
    auto outvec = mat.matVecMult(solution);
    print_vector(outvec, size);

    cout << "b: " << endl;
    print_vector(RHS, size);

    auto err = compute_error(RHS, outvec, size);
    cout << "absolute L2 error for Gauss-Seidal: " << err << endl << endl;

    cout << "***********************************************" << endl;
    cout << "Choose the next step: " << endl;
    cout << endl;
    cout << " 1. back to the page of create/load dense matrix " << endl;
    cout << " 2. back to the page of choosing dense/sparse" << endl;
    cout << " x: Exit" << endl << endl;
    cout << " >> ";
    string select;
    cin >> select;
    cout << "***********************************************" << endl;

    // string to char
    char *select_char = new char[1];
    if (select.size() != 1) { *select_char = '#'; }
    else { copy(select.begin(), select.end(), select_char); }

    switch (*select_char) {
        case '1':
            initDense();
            break;
        case '2':
            selectFormat();
            break;
        case 'x':
            exit(0);
        default:
            invalid("invalid, please enter again");
            selectFormat();
    }

    delete[] select_char;
    delete[] outvec;
    delete[] solution;
    delete[] RHS;
}

template<class T>
void Interface::solver_dense_LU(Matrix<T> &mat, T *RHS, T *solution, int size) {
    auto solver = new Solver<double>();

    clock_t start = clock();
    solver->dense_LU(mat, RHS, solution);
    clock_t end = clock();
    cout << "------------------------------------------------: " << endl << endl;
    cout << "Results for LU factorization: " << endl;
    cout << "--------------------------: " << endl << endl;
    cout << "time spent(LU): " << (double) (end - start) / CLOCKS_PER_SEC * 1000.0 << endl;
    cout << "x: " << endl;
    print_vector(solution, size);
    cout << "back subsititution : " << endl;
    auto outvec = mat.matVecMult(solution);
    print_vector(outvec, size);

    cout << "b: " << endl;
    print_vector(RHS, size);

    auto err = compute_error(RHS, outvec, size);
    cout << "absolute L2 error for LU-factorization: " << err << endl << endl;

    cout << "***********************************************" << endl;
    cout << "Choose the next step: " << endl;
    cout << endl;
    cout << " 1. back to the page of create/load dense matrix " << endl;
    cout << " 2. back to the page of choosing dense/sparse" << endl;
    cout << " x: Exit" << endl << endl;
    cout << " >> ";
    string select;
    cin >> select;
    cout << "***********************************************" << endl;

    // string to char
    char *select_char = new char[1];
    if (select.size() != 1) { *select_char = '#'; }
    else { copy(select.begin(), select.end(), select_char); }

    switch (*select_char) {
        case '1':
            initDense();
            break;
        case '2':
            selectFormat();
            break;
        case 'x':
            exit(0);
        default:
            invalid("invalid, please enter again");
            selectFormat();
    }

    delete[] select_char;
    delete[] outvec;
    delete[] solution;
    delete[] RHS;

}

template<class T>
void Interface::solver_BiCGSTAB(Matrix<T> &mat, T *RHS, T *solution, int size) {
    auto solver = new Solver<double>();

    clock_t start = clock();
    solver->BiCGSTAB(mat, RHS, solution);
    clock_t end = clock();
    cout << "time spent(BiCGSTAB): " << (double) (end - start) / CLOCKS_PER_SEC * 1000.0 << endl;

    mat.printMatrix();
    cout << "Results for BiCGSTAB factorization: " << endl;
    cout << "--------------------------: " << endl << endl;
    cout << "x: " << endl;
    print_vector(solution, size);
    cout << "back subsititution : " << endl;
    auto outvec = mat.matVecMult(solution);
    print_vector(outvec, size);

    cout << "b: " << endl;
    print_vector(RHS, size);

    auto err = compute_error(RHS, outvec, size);
    cout << "absolute L2 error for BiCGSTAB: " << err << endl << endl;

    cout << "***********************************************" << endl;
    cout << "Choose the next step: " << endl;
    cout << endl;
    cout << " 1. back to the page of create/load dense matrix " << endl;
    cout << " 2. back to the page of choosing dense/sparse" << endl;
    cout << " x: Exit" << endl << endl;
    cout << " >> ";
    string select;
    cin >> select;
    cout << "***********************************************" << endl;

    // string to char
    char *select_char = new char[1];
    if (select.size() != 1) { *select_char = '#'; }
    else { copy(select.begin(), select.end(), select_char); }

    switch (*select_char) {
        case '1':
            initDense();
            break;
        case '2':
            selectFormat();
            break;
        case 'x':
            exit(0);
        default:
            invalid("invalid, please enter again");
            selectFormat();
    }

    delete[] select_char;
    delete[] outvec;
    delete[] solution;
    delete[] RHS;

}

template<class T>
void Interface::solver_all(Matrix<T> &mat, T *RHS, int size) {
    srand((unsigned) time(NULL));

    auto x = new double[size]; // Gauss
    auto x2 = new double[size]; // BiCGSTAB
    auto x3 = new double[size]; // LU
    auto x4 = new double[size]; // Jacobi

    auto solver = new Solver<double>();


    cout << "------------------------------------------------ " << endl;
    cout << "Results for Gauss-Seidal: " << endl;
    cout << "--------------------------: " << endl << endl;
    clock_t start2 = clock();
    solver->dense_Gauss_Seidel(mat, RHS, x);
    clock_t end2 = clock();

    cout << "x: " << endl;
    print_vector(x, size);

    cout << "back substitutions Gauss-Seidal: " << endl;

    auto outvec = mat.matVecMult(x);
    print_vector(outvec, size);

    cout << "b: " << endl;
    print_vector(RHS, size);

    auto errGS = compute_error(RHS, outvec, size);

    cout << "absolute L2 error for Gauss-Seidal: " << errGS << endl << endl;

    cout << "---------------------------------------------- " << endl;
    cout << "Results for BiCGSTAB: " << endl;
    cout << "--------------------------: " << endl << endl;
    clock_t start3 = clock();
    solver->BiCGSTAB(mat, RHS, x2);
    clock_t end3 = clock();

    cout << "x: " << endl;
    print_vector(x2, size);

    cout << "back substitutions BiCGSTAB : " << endl;

    auto outvec2 = mat.matVecMult(x2);
    print_vector(outvec2, size);

    cout << "b: " << endl;
    print_vector(RHS, size);

    auto errBiCG = compute_error(RHS, outvec2, size);

    cout << "absolute L2 error for BiCGSTAB: " << errBiCG << endl << endl;

    cout << "---------------------------------------------- " << endl;
    cout << "Results for LU: " << endl;
    cout << "--------------------------: " << endl << endl;
    clock_t start4 = clock();
    solver->dense_LU(mat, RHS, x3);
    clock_t end4 = clock();
    cout << "x: " << endl;
    print_vector(x3, size);

    cout << "back substitutions LU: " << endl;

    auto outvec3 = mat.matVecMult(x3);
    print_vector(outvec3, size);

    cout << "b: " << endl;
    print_vector(RHS, size);

    auto errLU = compute_error(RHS, outvec3, size);

    cout << "absolute L2 error for LU: " << errLU << endl << endl;

    cout << "---------------------------------------------- " << endl;
    cout << "Results for Jacobi: " << endl;
    cout << "--------------------------: " << endl << endl;
    clock_t start5 = clock();
    solver->dense_Jacobi(mat, RHS, x4);
    clock_t end5 = clock();

    cout << "x: " << endl;
    print_vector(x4, size);

    cout << "back substitutions Jacobi: " << endl;

    auto outvec4 = mat.matVecMult(x4);
    print_vector(outvec4, size);

    cout << "b: " << endl;
    print_vector(RHS, size);

    auto errJa = compute_error(RHS, outvec4, size);

    cout << "absolute L2 error for Jacobi: " << errJa << endl << endl;

    cout << "time spent(Gauss-seidal): " << (double) (end2 - start2) / CLOCKS_PER_SEC * 1000.0 << endl;
    cout << "time spent(BiCGSTAB): " << (double) (end3 - start3) / CLOCKS_PER_SEC * 1000.0 << endl;
    cout << "time spent(LU-factorization): " << (double) (end4 - start4) / CLOCKS_PER_SEC * 1000.0 << endl;
    cout << "time spent(Jacobi): " << (double) (end5 - start5) / CLOCKS_PER_SEC * 1000.0 << endl;

    cout << "***********************************************" << endl;
    cout << "Choose the next step: " << endl;
    cout << endl;
    cout << " 1. back to the page of create/load dense matrix " << endl;
    cout << " 2. back to the page of choosing dense/sparse" << endl;
    cout << " x: Exit" << endl << endl;
    cout << " >> ";
    string select;
    cin >> select;
    cout << "***********************************************" << endl;

    // string to char
    char *select_char = new char[1];
    if (select.size() != 1) { *select_char = '#'; }
    else { copy(select.begin(), select.end(), select_char); }

    switch (*select_char) {
        case '1':
            initDense();
            break;
        case '2':
            selectFormat();
            break;
        case 'x':
            exit(0);
        default:
            invalid("invalid, please enter again");
            selectFormat();
    }

    delete[] select_char;
    delete[] RHS;

    delete[] outvec;
    delete[] outvec2;
    delete[] outvec3;
    delete[] outvec4;
    delete[] x;
    delete[] x2;
    delete[] x3;
    delete[] x4;
}

template<class T>
T *Interface::generate_random_vector(T *vec, int size) {
    for (int i = 0; i < size; i++) {
        vec[i] = T(rand() % 1000 - 500);
    }
    return vec;
}

template<class T>
void Interface::print_vector(T *vec, int size) {
    for (int i = 0; i < size; i++) {
        cout << vec[i] << " ";
    }
    cout << endl;
}

int Interface::read_row(const char *filename) {
    std::ifstream infile;
    infile.open(filename);
    if (!infile) {
        cout << filename << "not found the Matrix.txt file";//Determine whether the input file exists
        exit(1);
        return -1;
    } else {
        char c;
        c = infile.get();
        int count_row = 0;
        if (infile.eof()) {
            cout << filename << " Matrix.txt is empty";// Determine whether the input file is empty
            exit(1);
            return -1;
        } else {
            while (!infile.eof()) {
                c = infile.get();
                if (c == '\n' || c == '\r')
                    count_row++;
            }
            infile.close();
            return count_row + 1;
        }

    }
};


// Read .txt file to store in vector<vector<>>
vector<std::vector<double> > Interface::readMatrixFile(int matRow, const char *fileName) {
    std::vector<std::vector<double> > matrixALL{};
    //int row = 0;
    std::ifstream fileStream;
    std::string tmp;
    int count = 0;// count the row
    fileStream.open(fileName, std::ios::in);// only read

    if (fileStream.fail())//
    {
        throw std::logic_error("read file fail");
    } else//
    {
        while (getline(fileStream, tmp, '\n'))//row
        {
            std::cout << tmp << std::endl;
            int row = matRow;
            std::vector<double> tmpV{};
            std::istringstream is(tmp);
            for (int i = 0; i < row; i++) {
                std::string str_tmp;
                is >> str_tmp;
                tmpV.push_back(std::stod(str_tmp));
            }
            matrixALL.push_back(tmpV);
            count++;
        }
        fileStream.close();
    }

    return matrixALL;
}

void Interface::loadDenseFirst() {
    cout << "This method will read matrix A in 'Matrix_A.txt file' AND \n"
            "vector b in 'Vector_b.txt file" << endl;
    cout << "Please follow the follwing rules: \n"
            "1. the matrix in Matrix_A.txt is a positive definite matrix and enter by column major\n"
            "2. the rows of matrix should be equals to the elements number in vector" << endl;
    cout << endl;
    cout << "Example Matrix_A.txt:\n"
            "1 4 7\n2 5 8 \n3 6 9 " << endl;
    cout << "Example Vector_b.txt:\n"
            "1 2 3" << endl;
    cout << endl;
    cout << "Continue?  " << endl;
    cout << " y: continue" << endl;
    cout << " b: back" << endl;
    cout << " x: Exit" << endl << endl;
    cout << " >> ";
    string select;
    cin >> select;

    char *select_char = new char[1];
    if (select.size() != 1) { *select_char = '#'; }
    else { copy(select.begin(), select.end(), select_char); }

    switch (*select_char) {
        case 'y':
            loadDense();
            break;
        case 'b':
            cin.ignore();
            initDense();
            break;
        case 'x':
            exit(0);
        default:
            invalid("please enter again");
            initDense();
    }
    delete[] select_char;
}

void Interface::loadDense() {
    int rowsOfMat = 0;
    rowsOfMat = read_row("Matrix_A.txt");
    int size = rowsOfMat;
    cout << "The number of columns / rows in .txt file: " << size << endl;
    cout << "The matrix in this file:  " << endl;

    auto *mat_experi = new Matrix<double>(size, size, true);
    std::vector<std::vector<double>> matrixALL = readMatrixFile(size, "Matrix_A.txt");

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            mat_experi->values[i + j * size] = matrixALL[i][j];
        }
    }

    cout << "The vector in this file:  " << endl;
    std::vector<std::vector<double>> vecb = readMatrixFile(size, "Vector_b.txt");
    auto b = new double[size];
    for (int i = 0; i < size; i++) {
        b[i] = vecb[0][i];
    }

    cout << endl;
    auto x = new double[size];
    select_denseSolver(*mat_experi, b, x, size);


}

// ------------------------------------ DENSE  ----------------------------------------




// ------------------------------------ SPARSE  ----------------------------------------

void Interface::initCSR() {
    cout << "***********************************************" << endl;
    cout << "Select operation for CSR matrix A and vector b " << endl;
    cout << endl;
    cout << " 1. Randomly Create a test example" << endl;
    cout << " 2. Load a CSR matrix and a vector by .txt" << endl;
    cout << " b: Back" << endl;
    cout << " x: Exit" << endl << endl;
    cout << " >> ";
    string select;
    cin >> select;
    cout << "***********************************************" << endl;

    // string to char
    char *select_char = new char[1];
    if (select.size() != 1) { *select_char = '#'; }
    else { copy(select.begin(), select.end(), select_char); }

    switch (*select_char) {
        case '1':
            csr_create();
            break;
        case '2':
            loadCSRFirst();
            break;
        case 'b':
            cin.ignore();
            selectFormat();
            break;
        case 'x':
            exit(0);
        default:
            invalid("please enter again");
            initCSR();
    }
    delete[] select_char;
}

void Interface::csr_create() {
    cout << endl;
    cout << "Please enter the size of CSR matrix A: ";
    int input_size = 0;
    cin >> input_size;
    int size = input_size;
    if (cin.fail()) {
        invalid(" Please enter only integer");
        csr_create();
    }

    auto mat_new = new CSRMatrix<double>(size, size, 0, false);

    mat_new->generate_positive_definite();

    mat_new->printMatrix();

    auto mat_dense_new = mat_new->sparse2dense();

    auto b = new double[size];

    generate_random_vector(b, size);
    cout << "The RHS vector generated: " << endl;
    print_vector(b, size);

    auto x = new double[size];

    for (int i = 0; i < size; i++) {
        x[i] = 0;
    }

    select_sparseSolver(*mat_new, b, x, size);

    delete[] x;
    delete[] b;
}

template<class T>
void Interface::select_sparseSolver(CSRMatrix<T> &mat, T *RHS, T *solution, int size) {
    cout << "***********************************************" << endl;
    cout << "There have 2 solvers for CSR matix in this program " << endl;
    cout << " 1. sparse_Jacobi " << endl;
    cout << " 2. sparse_Gauss Seidel " << endl;
    cout << " a. compare 2 methods and get the feedback" << endl;
    cout << " x: Exit" << endl << endl;
    cout << "Please choose a method or enter 'a' to run all solvers" << endl;
    cout << " >> ";
    string select;
    cin >> select;
    cout << "***********************************************" << endl;
    // string to char
    char *select_char = new char[1];
    if (select.size() != 1) { *select_char = '#'; }
    else { copy(select.begin(), select.end(), select_char); }

    switch (*select_char) {
        case '1':
            solver_sparse_Jacobi(mat, RHS, solution, size);
            break;
        case '2':
            solver_sparse_GaussSeidel(mat, RHS, solution, size);
            break;
        case 'a':
            solver_sparse_all(mat, RHS, size);
            break;
        case 'x':
            exit(0);
        default:
            invalid("please enter again");
            initCSR();
    }
    delete[] select_char;
}

template<class T>
void Interface::solver_sparse_GaussSeidel(CSRMatrix<T> &mat, T *RHS, T *solution, int size) {
    auto solver = new Solver<double>();

    clock_t start = clock();
    solver->sparse_Gauss_Seidel(mat, RHS, solution);
    clock_t end = clock();
    cout << "time spent(sparse_Gauss_Seidel): " << (double) (end - start) / CLOCKS_PER_SEC * 1000.0 << endl;

    mat.printMatrix();
    cout << "Results for sparse_Gauss_Seidel: " << endl;
    cout << "--------------------------: " << endl << endl;
    cout << "x: " << endl;
    print_vector(solution, size);

    auto mulvec = new double[size];
    mat.matVecMult(solution, mulvec);
    cout << "back subsititution : " << endl;
    print_vector(mulvec, size);

    cout << "b: " << endl;
    print_vector(RHS, size);

    auto err = compute_error(RHS, mulvec, size);
    cout << "absolute L2 error for sparse_Gauss_Seidel: " << err << endl << endl;

    cout << "***********************************************" << endl;
    cout << "Choose the next step: " << endl;
    cout << endl;
    cout << " 1. back to the page of create/load sparse matrix " << endl;
    cout << " 2. back to the page of choosing dense/sparse" << endl;
    cout << " x: Exit" << endl << endl;
    cout << " >> ";
    string select;
    cin >> select;
    cout << "***********************************************" << endl;

    // string to char
    char *select_char = new char[1];
    if (select.size() != 1) { *select_char = '#'; }
    else { copy(select.begin(), select.end(), select_char); }

    switch (*select_char) {
        case '1':
            initCSR();
            break;
        case '2':
            selectFormat();
            break;
        case 'x':
            exit(0);
        default:
            invalid("invalid, please enter again");
            selectFormat();
    }

    delete[] select_char;
    delete[] mulvec;
    delete[] solution;
    delete[] RHS;


}

template<class T>
void Interface::solver_sparse_Jacobi(CSRMatrix<T> &mat, T *RHS, T *solution, int size) {
    auto solver = new Solver<double>();

    clock_t start = clock();
    solver->sparse_Jacobi(mat, RHS, solution);
    clock_t end = clock();
    cout << "time spent(sparse_Gauss_Jacobi): " << (double) (end - start) / CLOCKS_PER_SEC * 1000.0 << endl;

    mat.printMatrix();
    cout << "Results for sparse_Gauss_Jacobi: " << endl;
    cout << "--------------------------: " << endl << endl;
    cout << "x: " << endl;
    print_vector(solution, size);

    auto mulvec = new double[size];
    mat.matVecMult(solution, mulvec);
    cout << "back subsititution : " << endl;
    print_vector(mulvec, size);

    cout << "b: " << endl;
    print_vector(RHS, size);

    auto err = compute_error(RHS, mulvec, size);
    cout << "absolute L2 error for sparse_Gauss_Jacobi: " << err << endl << endl;

    cout << "***********************************************" << endl;
    cout << "Choose the next step: " << endl;
    cout << endl;
    cout << " 1. back to the page of create/load dense matrix " << endl;
    cout << " 2. back to the page of choosing dense/sparse" << endl;
    cout << " x: Exit" << endl << endl;
    cout << " >> ";
    string select;
    cin >> select;
    cout << "***********************************************" << endl;

    // string to char
    char *select_char = new char[1];
    if (select.size() != 1) { *select_char = '#'; }
    else { copy(select.begin(), select.end(), select_char); }

    switch (*select_char) {
        case '1':
            initCSR();
            break;
        case '2':
            selectFormat();
            break;
        case 'x':
            exit(0);
        default:
            invalid("invalid, please enter again");
            selectFormat();
    }

    delete[] select_char;
    delete[] mulvec;
    delete[] solution;
    delete[] RHS;


}

template<class T>
void Interface::solver_sparse_all(CSRMatrix<T> &mat, T *RHS, int size) {
    srand((unsigned) time(NULL));

    auto x = new double[size];  // to store the result of sparse Gauss
    auto x2 = new double[size]; // to store the result of sparse Jacobi
    auto x3 = new double[size]; // to store the result of dense Gauss
    auto x4 = new double[size]; // to store the result of dense Jacobi

    auto solver = new Solver<double>();

    cout << "b: " << endl;
    print_vector(RHS, size);

    auto densemat = mat.sparse2dense();

    cout << "------------------------------------------------ " << endl;
    cout << "Results for sparse Gauss-Seidal: " << endl;
    cout << "--------------------------: " << endl << endl;
    clock_t start2 = clock();
    solver->sparse_Gauss_Seidel(mat, RHS, x);
    clock_t end2 = clock();

    cout << "Solution: " << endl;
    print_vector(x, size);

    auto mulvec = new double[size];
    mat.matVecMult(x, mulvec);

    auto errsGS = compute_error(RHS, mulvec, size);


    cout << "---------------------------------------------- " << endl;
    cout << "Results for sparse Jacobi: " << endl;
    cout << "--------------------------: " << endl << endl;
    clock_t start3 = clock();
    solver->sparse_Jacobi(mat, RHS, x2);
    clock_t end3 = clock();

    cout << "Solution: " << endl;
    print_vector(x2, size);

    auto mulvec2 = new double[size];
    mat.matVecMult(x, mulvec2);

    auto errsJa = compute_error(RHS, mulvec2, size);


    cout << "---------------------------------------------- " << endl;
    cout << "Results for dense Gauss-Seidal (converting the input CSRMatrix to dense format): " << endl;
    cout << "--------------------------: " << endl << endl;
    clock_t start4 = clock();
    solver->dense_Gauss_Seidel(*densemat, RHS, x3);
    clock_t end4 = clock();
    cout << "Solution: " << endl;
    print_vector(x3, size);


    auto outvec3 = (*densemat).matVecMult(x3);

    auto errGS = compute_error(RHS, outvec3, size);

    cout << "---------------------------------------------- " << endl;
    cout << "Results for dense Jacobi (converting the input CSRMatrix to dense format): " << endl;
    cout << "--------------------------: " << endl << endl;
    clock_t start5 = clock();
    solver->dense_Jacobi(*densemat, RHS, x4);
    clock_t end5 = clock();
    cout << "Solution: " << endl;
    print_vector(x4, size);


    auto outvec4 = (*densemat).matVecMult(x4);

    auto errJa = compute_error(RHS, outvec4, size);

    cout << "absolute L2 error for sparse Gauss-Seidal: " << errsGS << endl << endl;
    cout << "absolute L2 error for sparse Jacobi: " << errsJa << endl << endl;
    cout << "absolute L2 error for dense Gauss-Seidal: " << errsGS << endl << endl;
    cout << "absolute L2 error for dense Jacobi: " << errJa << endl << endl;

    cout << "sparseness of the Matrix (number of non-zeros / total elements): " << double(mat.nnzs) / (size * size)
         << endl;
    cout << "time spent(sparse Gauss-Seidal): " << (double) (end2 - start2) / CLOCKS_PER_SEC * 1000.0 << endl;
    cout << "time spent(sparse Jacobi): " << (double) (end3 - start3) / CLOCKS_PER_SEC * 1000.0 << endl;
    cout << "time spent(dense Gauss-Seidal): " << (double) (end4 - start4) / CLOCKS_PER_SEC * 1000.0 << endl;
    cout << "time spent(dense Jacobi): " << (double) (end5 - start5) / CLOCKS_PER_SEC * 1000.0 << endl;

    cout << "***********************************************" << endl;
    cout << "Choose the next step: " << endl;
    cout << endl;
    cout << " 1. back to the page of create/load sparse matrix " << endl;
    cout << " 2. back to the page of choosing dense/sparse" << endl;
    cout << " x: Exit" << endl << endl;
    cout << " >> ";
    string select;
    cin >> select;
    cout << "***********************************************" << endl;

    // string to char
    char *select_char = new char[1];
    if (select.size() != 1) { *select_char = '#'; }
    else { copy(select.begin(), select.end(), select_char); }

    switch (*select_char) {
        case '1':
            initCSR();
            break;
        case '2':
            selectFormat();
            break;
        case 'x':
            exit(0);
        default:
            invalid("invalid, please enter again");
            selectFormat();
    }

    delete[] select_char;
    delete[] RHS;

    delete[] mulvec;
    delete[] mulvec2;
    delete[] outvec3;
    delete[] outvec4;
    delete[] x;
    delete[] x2;
    delete[] x3;
    delete[] x4;
}

vector<std::vector<double> > Interface::read_csr(string pathname) {
    std::vector<std::vector<double> > csrAll{};

    ifstream infile;
    infile.open(pathname.data());   //Connect the file stream object to the file
    assert(infile.is_open());   //If this fails, an error message is printed and the program is terminated
    vector<double> suanz;
    string s;
    while (getline(infile, s)) {
        istringstream is(s); //To convert a read row into a data stream for operation
        double d;
        while (!is.eof()) {
            is >> d;
            suanz.push_back(d);
        }
        csrAll.push_back(suanz);
        suanz.clear();
        s.clear();
    }
    infile.close(); //Close the file input stream
    return csrAll;
}

void Interface::loadCSRFirst() {
    cout << "This method will read CSR matrix A in 'csr.txt file' AND \n"
            "vector b in 'csrVector.txt file" << endl;
    cout << endl;
    cout << "Example csr.txt:\n"
            "9819 -6 -125 -240 65 5333 6 160 -166 -4 3358 -160 -320 160 240 6400<-no space here\n"
            "0 4 8 12 16<-no space here\n"
            "0 1 2 3 0 1 2 3 0 1 2 3 0 1 2 3<-no space here  " << endl;
    cout << endl;
    cout << "Example Vector_b.txt:\n"
            "1 2 3 4 5 " << endl;
    cout << endl;
    cout << "Continue?  " << endl;
    cout << " y: continue" << endl;
    cout << " b: back" << endl;
    cout << " x: Exit" << endl << endl;
    cout << " >> ";
    string select;
    cin >> select;

    char *select_char = new char[1];
    if (select.size() != 1) { *select_char = '#'; }
    else { copy(select.begin(), select.end(), select_char); }

    switch (*select_char) {
        case 'y':
            loadCSR();
            break;
        case 'b':
            cin.ignore();
            initCSR();
            break;
        case 'x':
            exit(0);
        default:
            invalid("please enter again");
            initDense();
    }
    delete[] select_char;
}

void Interface::loadCSR() {
    int rowsOftxt = 0;
    rowsOftxt = read_row("csr.txt");
    if (rowsOftxt != 3) {
        cout << "The row of CSR matrix A in txt:" << rowsOftxt << endl;
        invalid(" The row of matrix A not equals to 3, please check your input file");
        loadCSRFirst();
    } else {

        cout << "The matrix in this file:  " << endl;
        std::vector<std::vector<double>> csrALL = read_csr("csr.txt");

        int coutRow = 0;
        int coutCol = 0;
        double nnzs_ = 0;
        int size = 0;

        for (auto a : csrALL) {
            for (auto b : a) {
                if (coutRow == 0) {
                    nnzs_ += 1;
                }
                if (coutRow == 1) {
                    size += 1;
                }
                cout << b << " ";
            }
            coutRow += 1;
            cout << endl;
        }

        size = size - 1;
        auto mat_new = new CSRMatrix<double>(size, size, nnzs_, true);
        cout << "Information of this CSR matrix: " << endl;
        cout << "size: " << size << endl;
        cout << "nnzs:" << nnzs_ << endl;

        coutRow = 0;
        int index_va = 0;
        int ind_rowpo = 0;
        int ind_col = 0;

        for (auto a : csrALL) {
            for (auto b : a) {
                if (coutRow == 0) {
                    mat_new->values[index_va] = b;
                    index_va = index_va + 1;
                }
                if (coutRow == 1) {
                    mat_new->row_position[ind_rowpo] = b;
                    ind_rowpo = ind_rowpo + 1;
                }
                if (coutRow == 2) {
                    mat_new->col_index[ind_col] = b;
                    ind_col = ind_col + 1;
                }
            }
            coutRow += 1;
            cout << coutRow << endl;
        }

        mat_new->printMatrix();

        auto mat_dense_new = mat_new->sparse2dense();


        cout << "The vector in this file:  " << endl;
        std::vector<std::vector<double>> vecb = readMatrixFile(size, "csrVector.txt");
        auto b = new double[size];
        for (int i = 0; i < size; i++) {
            b[i] = vecb[0][i];
        }

        auto x = new double[size];

        select_sparseSolver(*mat_new, b, x, size);

        delete[] x;
        delete[] b;

    }
}
// ------------------------------------ SPARSE  ----------------------------------------

// invalid output
void Interface::invalid(std::string message) {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << endl << " Invalid" << endl << endl;
    cout << "------------------------------" << endl;
    cout << message << endl;
    cout << endl;
    cout << " Press enter to return..." << endl;
    cin.get();
}




