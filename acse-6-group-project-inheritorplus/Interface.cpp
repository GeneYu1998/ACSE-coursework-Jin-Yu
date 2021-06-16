#include "Interface.h"
//#include "PNG.h"
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include "Serial.cpp"
#include "Parallel.cpp"
#include "PNG.cpp"
#include "Image.cpp"
#include "Pixel.cpp"


//#include "Image.h"

Interface::Interface() {
    Intro();
}

Interface::~Interface() {
    //delete[] Img;
}

void Interface::Intro() {
    //system("CLS");
    cin.clear();
    cout << endl;
    cout << "---------------------------------------------------" << endl;
    cout << " ACSE-6 OpenMP Group Assignment" << endl;
    cout << " The Game of Life" << endl << endl;
    cout << "-------------------------------" << endl;
    cout << " Yuchen Wang" << endl << " Jin Yu" << endl << " Xuyuan Chen" << endl;
    cout << endl;
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
        this->serial = *new Serial(size, max_steps);
        serial.maxiter = itmax;
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

    char select;
    cin >> select;

    switch (select) {
        case '1':
            numOfthreads();
        case '2':
            pureRunParallel();
        default:
            compareOrNot();
    }
    cout << "***********************************************" << endl;
}

void Interface::numOfthreads() {
    int thread_num;
    cout << endl;
    cout << "***********************************************" << endl;
    cout << endl;
    cout << "Please enter the number of threads to use for OpenMP: ";
    cin >> thread_num;
    if (cin.fail()) {
        InvalidInput("Please enter an integer");
        numOfthreads();
    } else {
        this->threads = thread_num;
        this->parallel = *new Parallel(size, thread_num);
        cout << "threads avaliable: " << threads << endl;
        Compare();
        cout << "***********************************************" << endl;
    }
}

void Interface::pureRunParallel() {
    cout << endl;

    printf("Running %d iterations for a %d * %d periodic mesh (parallel): \n", max_steps, size, size);

    cout << "---------------------------------------------------" << endl;

    srand(time(NULL));

    cout << "Results for omp_get_max_threads() on this PC: " << omp_get_max_threads() << endl;

    cout << endl;
    parallel = *new Parallel(size, 1);

    if (load == 0) {
        parallel.set_initial();
    }else{
        serial.grid = readFile(size, "input.txt");
        parallel.convert_initial(serial.grid, parallel.value, size);
    }


    for (int i = 1; i < omp_get_max_threads() * 2; ++i) {
        parallel.threads = i;
        double start = omp_get_wtime();
        parallel.get_index_row_col();
        parallel.set_zero(parallel.neigh_value);
        for (int n = 0; n < max_steps; n++) {
            serial.gridToPNG(n);
            parallel.do_iteration_parallel();
        }
        double end = omp_get_wtime();


        cout << " Time used for " << i << " thread(s) :" << end - start << " s " << endl;
        cout << endl;
    }


    cout << " \n\n Image saved to ./data folder" << endl << endl;
    cout << "\ntxt file saved to ./output folder" << endl << endl;

    back_page();

}

void Interface::Compare() {

    cout << endl;

    printf("Running %d iterations for a %d * %d periodic mesh: \n", max_steps, size, size);

    cout << "---------------------------------------------------" << endl;

    srand(time(NULL));

    cout << "threads avaliable: " << parallel.threads << endl;

    cout << endl;

    serial.grid.resize(size, vector<bool>(size));
    serial.new_grid.resize(size, vector<bool>(size));


    double start2 = omp_get_wtime();
    parallel.get_index_row_col_serial();
    double end2 = omp_get_wtime();

    double start1 = omp_get_wtime();
    parallel.get_index_row_col();
    double end1 = omp_get_wtime();

    cout << " get row, col indexes use (parallel): " << end1 - start1 << " us" << endl;
    cout << " get row, col indexes use (serial): " << end2 - start2 << " us" << endl;

    cout << endl;

    //set an initial random collection of points
    if (load == 0) {
        double start3 = omp_get_wtime();
        serial.randomInit();
        double end3 = omp_get_wtime();


        double start4 = omp_get_wtime();
        parallel.set_initial();
        double end4 = omp_get_wtime();
        cout << size << " * " << size << " grid set value use (parallel): " << end4 - start4 << " us" << endl;
        cout << size << " * " << size << " grid set value use (serial): " << end3 - start3 << " us" << endl;
    } else{
        serial.grid = readFile(size, "input.txt");
    }

    parallel.convert_initial(serial.grid, parallel.value, size);
    cout << endl;
//    cout << "grid init: " << endl;
//    for (int i = 0; i < imax; i++)
//    {
//        for (int j = 0; j < jmax; j++)
//            cout << serial.grid[i][j];
//        cout << endl;
//    }

//    cout << "value init: " << endl;
//
//    for (int i = 0; i < imax; i++)
//    {
//        for (int j = 0; j < jmax; j++)
//            cout << parallel.value[i + j * jmax];
//        cout << endl;
//    }


    cout << "Error before iterations: ";
    parallel.error(serial.grid, parallel.value, size);
    cout << endl;

    double start5 = omp_get_wtime();
    for (int n = 0; n < max_steps; n++) {
        serial.gridToPNG(n);
        serial.do_iteration();
        //
    }
    double end5 = omp_get_wtime();


//    cout << "grid result: " << endl;
//    for (int i = 0; i < imax; i++)
//    {
//        for (int j = 0; j < jmax; j++)
//            cout << serial.grid[i][j];
//        cout << endl;
//    }

    cout << endl;
    cout << max_steps << " Iterations use (serial): " << end5 - start5 << " us" << endl;

    double start6 = omp_get_wtime();
    parallel.get_index_row_col();
    parallel.set_zero(parallel.neigh_value);
    for (int n = 0; n < max_steps; n++) {
        parallel.do_iteration_parallel();
    }
    double end6 = omp_get_wtime();

//    cout << "value result: " << endl;
//
//    for (int i = 0; i < imax; i++)
//    {
//        for (int j = 0; j < jmax; j++)
//            cout << parallel.value[i + j * jmax];
//        cout << endl;
//    }

    cout << max_steps << " Iterations use (parallell): " << end6 - start6 << " us" << endl;
    cout << endl;


    cout << "Error After iterations: ";
    parallel.error(serial.grid, parallel.value, size);

    cout << "\n\nImage saved to ./data folder" << endl;
    cout << "\ntxt file saved to ./output folder" << endl << endl;

    cout << " changing thread numbers? (y/n)" << endl;
    cout << " >> ";

    char select1;
    cin >> select1;

    switch (select1) {
        case 'y':
            numOfthreads();
        case 'n':
            back_page();
        default:
            exit(0);
    }
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
        case 'n':
            exit(0);
        default:
            exit(0);
    }
}


void Interface::initLoad() {
    cout << "This method will read initial conditions from 'input.txt' file"<< endl;
    cout << "Please follow the follwing rules: \n"
            "1. Use whitespace to separate 0 and 1 \n"
            "2. The rows of grid should be equals to the column of grid\n"
            "3. Make sure input.txt not empty before press 'y' " << endl;

    cout << endl;
    cout << "Example input.txt:\n"
            "1 0 0\n0 1 0 \n0 0 1 " << endl;
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
            load = 1;
            size = read_row("input.txt");
            initMaxIter();
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
            cout << filename << " Initial.txt is empty";// Determine whether the input file is empty
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
}



// Read .txt file to store in vector<vector<>>
vector<std::vector<bool> > Interface::readFile(int matRow, const char *fileName) {
    std::vector<std::vector<bool> > matrixALL{};
    //int row = 0;
    std::ifstream fileStream;
    std::string tmp;
    int count = 0;// count the row
    fileStream.open(fileName, std::ios::in);// only read

    if (fileStream.fail())//
    {
        throw std::logic_error("read file fail");
    } else
    {
        while (getline(fileStream, tmp, '\n'))//row
        {
            //std::cout << tmp << std::endl;
            int row = matRow;
            std::vector<bool> tmpV{};
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

void Interface::InvalidInput(string message) {
    // Ignore any of cin left
    cin.clear();
    //cin.ignore(numeric_limits<streamsize>::max(), '\n');
    //system("CLS");
    cout << " Invalid Selection" << endl;
    cout << "------------------------------" << endl;
    cout << message << endl;
    cout << endl;
    cout << " Press enter to return..." << endl;
    cin.get();
    cin.get();
}




