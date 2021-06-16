#pragma once
#include <string>
#include <iostream>
#include "Parallel.h"
#include "Serial.h"



using namespace std;

class Interface {
public:
    Interface();
    ~Interface();

private:
    void Intro();


    void InvalidInput(string message); // use for invalid operation

    void iniCondition();

    int size; // store the size of grid

    int max_steps; // how many steps the game should be run

    int threads; // set the threads for program

    int load = 0; // 0: randomly create the grid; 1: load the grid from txt file

    string path; // store the path of txt file

public:

    void initSizeOfGrid(); // set the size of grid

    void initMaxIter(); // set the steps

    void compareOrNot(); // run the single parallel version or compare to serial version

    void multiOrsingle(); // single run: run with a certain thread number; multi run: comparing performance with thread number from 1 to 24

    void numOfthreads(); // set the number of threads to use for  run serial and parallel

    void numOfthreads2(); // set the number of threads to use for single run

    void RunParallelSingle();

    void back_page(); // back to the init page

    void pureRunParallel(); // only run the parallel version of game

    void Compare(); // run both two version and compare the results

    int read_row(const char *filename); // get the rows of txt file

    void initLoad(); // make sure the user will format the file

    void continueLoad(); // progress the load txt

    void enterPath(); // input the path of txt file

    vector<vector<bool>> readFile(int matRow, const char *fileName); // Read .txt file to initial the grid or value (representation for grid in parallel)


    Serial* serial;
    Parallel* parallel;

    void defaultPath();
};

