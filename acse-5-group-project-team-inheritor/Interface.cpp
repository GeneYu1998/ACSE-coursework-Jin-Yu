/*
  Team: inheritorplus
  Github handle: acse-jy220; acse-xc520; acse-sw3620
*/

#include "Interface.h"
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include "Serial.cpp"
#include "Parallel.cpp"
#include "PNG.cpp"
#include "Image.cpp"
#include "Pixel.cpp"


using namespace std;


Interface::Interface() {
    Intro();
}

Interface::~Interface() {
    delete parallel;
    delete serial;
}

void Interface::Intro() {
    //system("CLS");
    cin.clear();
    cout << endl;
    cout << "---------------------------------------------------" << endl;
    cout << " ACSE-6 OpenMP Group Assignment" << endl;
    cout << " The Game of Life" << endl << endl;
    cout << "-------------------------------" << endl;
    cout << " Team-inheritorplus:" << endl;
    cout << " Jin Yu" << endl << " Xuyuan Chen" << endl << " Yuchen Wang" << endl;
    cout << endl;
    cout << " Github Link: https://github.com/acse-2020/group-project-inheritorplus" << endl;
    cout << "-------------------------------" << endl;
    cout << " This software is designed for implement\n"
            " a parallel version of Conway’s Game of Life using OpenMP" << endl;
    cout << "-------------------------------" << endl;
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
            Intro();
    }
    iniCondition();

}

void Interface::iniCondition() {
    cout << "***********************************************" << endl;
    cout << "Select operation for ConwaysGame initial condition " << endl;
    cout << endl;
    cout << " 1. Randomly create an initial example" << endl;
    cout << " 2. Load from .txt" << endl;
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
            load = 0;
            initSizeOfGrid();
            break;
        case '2':;
            initLoad();
            break;
        case 'b':
            cin.ignore();
            Intro();
            break;
        case 'x':
            exit(0);
        default:
            InvalidInput(" please enter again");
            iniCondition();
    }
    delete[] select_char;
}

void Interface::initSizeOfGrid() {

    int size;
    cout << endl;
    cout << "Please enter the rows/cols of the grid" << endl;
    cout << "(For example if you want to create a grid with size 100 * 100 \n"
            "you can input 100)" << endl;
    cout << " >> ";
    cin >> size;
    if (cin.fail()) {
        InvalidInput(" Please enter an integer");
        initSizeOfGrid();
    } else {
        this->size = size;
        cout << "size of grid: " << this->size << " * " << this->size << endl;
        cout << endl;
        cout << "***********************************************" << endl;

        initMaxIter();
    }
}

void Interface::initMaxIter() {
    int itmax;
    cout << "Please enter the max iteration of the game: ";
    cin >> itmax;
    if (cin.fail()) {
        InvalidInput(" Please enter an integer");
        iniCondition();
    } else {
        this->max_steps = itmax;
        cout << "max steps: " << max_steps << endl;
        compareOrNot();
        cout << "***********************************************" << endl;
    }
}

void Interface::compareOrNot() {
    cout << endl;
    cout << "***********************************************" << endl;
    cout << "(1) Run both parallel and serial versions for comparison. \n";
    cout << "(2) Simply run the parallel version. \n ";
    cout << ">> ";
    string select;
    cin >> select;
    char *select_char = new char[1];
    if (select.size() != 1) { *select_char = '#'; }
    else { copy(select.begin(), select.end(), select_char); }

    switch (*select_char) {
        case '1':
            numOfthreads();
            break;
        case '2':
            multiOrsingle();
            break;
        case 'x':
            exit(0);
        default:
            InvalidInput(" please enter again");
            compareOrNot();
    }

    delete[] select_char;
}

void Interface::numOfthreads() {
    int thread_num;
    cout << endl;
    cout << "***********************************************" << endl;
    cout << endl;
    cout << "Results for omp_get_max_threads() on this PC: " << omp_get_max_threads() << endl << endl;
    cout << "Please enter the number of threads to use for OpenMP: ";
    cin >> thread_num;
    // generating output with a default thread number first.        
    this->parallel = new Parallel(size, omp_get_max_threads());

    if (cin.fail()) {
        InvalidInput("Please enter an integer");
        numOfthreads();
    } else {
        if (load == 0) {
            parallel->set_initial();
            parallel->value_to_PNG(0);
        } else {
            vector<vector<bool>> grid = readFile(size, path.c_str());
            parallel->convert_initial(grid, parallel->value, size);
            parallel->value_to_PNG(0);
        }

        parallel->get_index_row_col();
        parallel->set_zero(parallel->neigh_value);

        for (int n = 1; n <= max_steps; n++) {
            parallel->do_iteration_parallel();
            if (n == (max_steps / 2) || n == max_steps) parallel->value_to_PNG(n);
        }

        parallel->threads = thread_num;

        Compare();
        cout << "***********************************************" << endl;
    }
}

void Interface::multiOrsingle() {
    cout << endl;
    cout << "***********************************************" << endl;
    cout << "Results for omp_get_max_threads() on this PC: " << omp_get_max_threads() << endl << endl;
    cout << "(1) a single run, with a certain thread number \n";
    cout << "(2) multiple runs, comparing performance with thread number from 1 to 2 * " << omp_get_max_threads()
         << " = " << 2 * omp_get_max_threads() << endl;
    cout << ">> ";
    string select;
    cin >> select;
    char *select_char = new char[1];
    if (select.size() != 1) { *select_char = '#'; }
    else { copy(select.begin(), select.end(), select_char); }

    switch (*select_char) {
        case '1':
            numOfthreads2();
            break;
        case '2':
            pureRunParallel();
            break;
        case 'x':
            exit(0);
        default:
            InvalidInput(" please enter again");
            multiOrsingle();
    }
    delete[] select_char;
    cout << "***********************************************" << endl;
}

void Interface::numOfthreads2() {
    int thread_num;
    cout << endl;
    cout << "***********************************************" << endl;
    cout << endl;
    cout << "Please enter the number of threads to use for OpenMP: ";
    cin >> thread_num;
    // generating output with a default thread number first.        
    this->parallel = new Parallel(size, omp_get_max_threads());
    if (cin.fail()) {
        InvalidInput("Please enter an integer");
        numOfthreads2();
    } else {
        if (load == 0) {
            parallel->set_initial();
            parallel->value_to_PNG(0);
        } else {
            vector<vector<bool>> grid = readFile(size, path.c_str());
            parallel->convert_initial(grid, parallel->value, size);
            parallel->value_to_PNG(0);
        }

        parallel->get_index_row_col();
        parallel->set_zero(parallel->neigh_value);

        for (int n = 1; n <= max_steps; n++) {
            parallel->do_iteration_parallel();
            if (n == (max_steps / 2) || n == max_steps) parallel->value_to_PNG(n);
        }
        parallel->threads = thread_num;
        RunParallelSingle();
        cout << "***********************************************" << endl;
    }
}

void Interface::RunParallelSingle() {

    cout << endl;

    printf("Running %d iterations for a %d * %d periodic mesh (parallel): \n", max_steps, size, size);

    cout << "---------------------------------------------------" << endl;

    cout << "threads avaliable: " << parallel->threads << endl;

    srand(time(NULL));

    cout << endl;


    // run for our chosed thread
    double start = omp_get_wtime();
    parallel->get_index_row_col();
    parallel->set_zero(parallel->neigh_value);

    for (int n = 1; n <= max_steps; n++) {
        parallel->do_iteration_parallel();
    }

    double end = omp_get_wtime();

    cout << " Time used for " << parallel->threads << " thread(s) :" << end - start << " s " << endl;
    cout << endl;

    delete parallel;

    cout << "\n\nImage saved to ./png folder, \n"
            "The black grid in the picture shows the surviving cells" << endl << endl;
    cout << "\ntxt file saved to ./txt folder" << endl << endl;

    back_page();
}

void Interface::pureRunParallel() {
    cout << endl;
    printf("Running %d iterations for a %d * %d periodic mesh (parallel): \n", max_steps, size, size);
    cout << "---------------------------------------------------" << endl;

    srand(time(NULL));

    cout << endl;
    parallel = new Parallel(size, 1);

    if (load == 0) {
        parallel->set_initial();
        parallel->value_to_PNG(0);
    } else {
        vector<vector<bool>> grid = readFile(size, path.c_str());
        parallel->convert_initial(grid, parallel->value, size);
        parallel->value_to_PNG(0);
    }

    // generating output with a default thread number first.
    parallel->get_index_row_col();
    parallel->set_zero(parallel->neigh_value);
    parallel->threads = omp_get_max_threads();

    for (int n = 1; n <= max_steps; n++) {

        parallel->do_iteration_parallel();
        if (n == (max_steps / 2) || n == max_steps) parallel->value_to_PNG(n);

    }

    // loop over different thread_nums
    for (int i = 1; i < omp_get_max_threads() * 2; ++i) {
        parallel->threads = i;
        double start = omp_get_wtime();
        parallel->get_index_row_col();
        parallel->set_zero(parallel->neigh_value);

        for (int n = 1; n <= max_steps; n++) {
            parallel->do_iteration_parallel();
        }

        double end = omp_get_wtime();

        cout << " Time used for " << i << " thread(s) :" << end - start << " s " << endl;
        cout << endl;
    }

    delete parallel;

    cout << "\n\nImage saved to ./png folder, \n"
            "The black grid in the picture shows the surviving cells" << endl << endl;
    cout << "\ntxt file saved to ./txt folder" << endl << endl;

    back_page();

}

void Interface::Compare() {
    this->serial = new Serial(size);

    cout << endl;

    printf("Running %d iterations for a %d * %d periodic mesh: \n", max_steps, size, size);

    cout << "---------------------------------------------------" << endl;

    srand(time(NULL));

    cout << "threads avaliable: " << parallel->threads << endl;

    cout << endl;

    serial->grid.resize(size, vector<bool>(size));
    serial->new_grid.resize(size, vector<bool>(size));

    double start2 = omp_get_wtime();
    parallel->get_index_row_col_serial();
    double end2 = omp_get_wtime();

    double start1 = omp_get_wtime();
    parallel->get_index_row_col();
    double end1 = omp_get_wtime();

    cout << " get row, col indexes use (parallel): " << end1 - start1 << " s" << endl;
    cout << " get row, col indexes use (serial): " << end2 - start2 << " s" << endl;

    cout << endl;

    // set an initial random collection of points
    if (load == 0) {
        double start3 = omp_get_wtime();
        serial->randomInit();
        double end3 = omp_get_wtime();

        double start4 = omp_get_wtime();
        parallel->set_initial();
        double end4 = omp_get_wtime();
        cout << size << " * " << size << " grid set value use (parallel): " << end4 - start4 << " s" << endl;
        cout << size << " * " << size << " grid set value use (serial): " << end3 - start3 << " s" << endl;
    }
        // read the initial condition from file
    else {
        serial->grid = readFile(size, path.c_str());
    }

    parallel->convert_initial(serial->grid, parallel->value, size);
    parallel->value_to_PNG(0);

    cout << endl;
    cout << "Error before iterations: ";
    parallel->error(serial->grid, parallel->value, size);
    cout << endl;

    double start5 = omp_get_wtime();
    for (int n = 1; n <= max_steps; n++) {
        serial->do_iteration();
    }
    double end5 = omp_get_wtime();


    cout << endl;
    cout << max_steps << " Iterations use (serial): " << end5 - start5 << " s" << endl;

    double start6 = omp_get_wtime();
    parallel->get_index_row_col();
    parallel->set_zero(parallel->neigh_value);
    for (int n = 1; n <= max_steps; n++) {
        parallel->do_iteration_parallel();
    }
    double end6 = omp_get_wtime();

    cout << max_steps << " Iterations use (parallell): " << end6 - start6 << " s" << endl;
    cout << endl;


    cout << "Error After iterations: ";
    parallel->error(serial->grid, parallel->value, size);

    delete serial;
    delete parallel;

    cout << "\n\nImage saved to ./png folder, \n"
            "The black grid in the picture shows the surviving cells" << endl << endl;
    cout << "\ntxt file saved to ./txt folder" << endl << endl;

    cout << " changing thread numbers? (y/n)" << endl;
    cout << " >> ";

    string select;
    cin >> select;

    // string to char
    char *select_char = new char[1];
    if (select.size() != 1) { *select_char = '#'; }
    else { copy(select.begin(), select.end(), select_char); }

    switch (*select_char) {
        case 'y':
            numOfthreads();
            break;
        case 'n':
            back_page();
            break;
        default:
            exit(0);
    }
    delete[] select_char;
}

void Interface::back_page() {
    cout << endl;
    cout << "***********************************************" << endl;
    cout << " Back to initial page? (y/n)" << endl;
    cout << " >> ";

    char select;
    cin >> select;

    switch (select) {
        case 'y':
            iniCondition();
            break;
        case 'n':
            exit(0);
        default:
            exit(0);
    }
}


void Interface::initLoad() {
    cout << "This method will read initial conditions from 'input.txt' file" << endl;
    cout << "Please follow the follwing rules: \n"
            "1. Use whitespace to separate 0 and 1 \n"
            "2. The rows of grid should be equals to the column of grid\n"
            "3. you can edit the input.txt file for read or\n"
            "   enter the right format of txt file address" << endl;
    cout << "---------------------------------------------------" << endl;
    cout << " Example input.txt:\n"
            " 1 0 0\n 0 1 0 \n 0 0 1 " << endl;
    cout << "---------------------------------------------------" << endl;
    cout << "Continue?  " << endl;
    cout << " y: continue" << endl;
    cout << " b: back" << endl;
    cout << " x: Exit" << endl;
    cout << " >> ";
    string select;
    cin >> select;
    char *select_char = new char[1];
    if (select.size() != 1) { *select_char = '#'; }
    else { copy(select.begin(), select.end(), select_char); }
    switch (*select_char) {
        case 'y':
            load = 1;
            continueLoad();
            break;
        case 'b':
            cin.ignore();
            iniCondition();
            break;
        case 'x':
            exit(0);
        default:
            InvalidInput("please enter again");
            initLoad();
    }
    delete[] select_char;
}

void Interface::continueLoad() {

    cout << " Insert txt address to load:" << endl;
    cout << " Example path：(1)./input.txt  (2) A custom path like ' ./txt/GenNum(0)_size(100*100).txt'. " << endl;
    cout << " Please enter 1 or 2" << endl;
    cout << " >> ";
    string select;
    cin >> select;
    char *select_char = new char[1];
    if (select.size() != 1) { *select_char = '#'; }
    else { copy(select.begin(), select.end(), select_char); }
    switch (*select_char) {
        case '1':
            defaultPath();
            break;
        case '2':
            enterPath();
            break;
        case 'x':
            exit(0);
        default:
            InvalidInput("please enter again");
            continueLoad();
    }
    delete[] select_char;
}

void Interface::defaultPath() {
    cout << " \n Loading txt..." << endl;
    cout << endl;
    int row;
    path = "./input.txt";
    cout << endl;
    row = read_row(path.c_str());
    size = row;
    cout << "size: " << size << " * " << size << endl;
    initMaxIter();
}

void Interface::enterPath() {
    string in_address;
    cout << "********************************************************************" << endl;
    cout << "Please enter the path: " << endl << endl;
    cout << " Example path：'./txt/GenNum(0)_size(100*100).txt' " << endl;
    cout << " >> ";
    cin >> in_address;
    if (cin.fail()) {
        InvalidInput("string only");
        enterPath();
    } else {
        cout << " \n Loading txt..." << endl;
        cout << endl;
        int row;
        path = in_address;
        row = read_row(path.c_str());
        size = row;
        initMaxIter();
    }
}

int Interface::read_row(const char *filename) {
    std::ifstream infile;
    infile.open(filename);
    if (!infile) {
        cout << endl;
        cout << "The file named " << filename << " not found !!! " << endl;//Determine whether the input file exists
        cout << endl << "back to the input setting page.................................." << endl << endl;
        cout << "-----------------------------------------------------" << endl;
        initLoad();
        return -1;
    } else {
        char c;
        int count_row = 0;
        if (infile.eof()) {
            cout << filename << " the file is empty !!!";// Determine whether the input file is empty
            cout << endl << endl << "back to the input setting page.................................." << endl << endl;
            cout << "-----------------------------------------------------" << endl;
            initLoad();
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
}

vector<std::vector<bool> > Interface::readFile(int gridRow, const char *fileName) {
    std::vector<std::vector<bool> > gridALL{};
    std::ifstream fileStream;
    std::string tmp;
    int count = 0; // count the row
    fileStream.open(fileName, std::ios::in); // only read

    if (fileStream.fail()) // fail open the file
    {
        throw std::logic_error("read file fail");
    } else {
        while (getline(fileStream, tmp, '\n')) // read each row
        {
            int row = gridRow;
            std::vector<bool> tmpV{};
            std::istringstream is(tmp);
            for (int i = 0; i < row; i++) {
                std::string str_tmp;
                is >> str_tmp;
                tmpV.push_back(std::stod(str_tmp));
            }
            gridALL.push_back(tmpV);
            count++;
        }
        fileStream.close();
    }

    return gridALL;
}


void Interface::InvalidInput(string message) {
    cin.clear();
    cout << " Invalid Selection" << endl;
    cout << "------------------------------" << endl;
    cout << message << endl;
    cout << endl;
    cout << " Press enter to return..." << endl;
    cin.get();
    cin.get();
}




