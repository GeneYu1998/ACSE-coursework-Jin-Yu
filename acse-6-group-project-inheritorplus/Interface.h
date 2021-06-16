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
    //Image * Img;



private:
    void Intro();
    void InvalidInput(string message);
    void iniCondition();
    int size;
    int max_steps;
    int threads;
    int load = 0; // 0: randomly create; 1: load

    //int img_type_; // 1: BMP; 2: PPM; 3: PNG
    // Parameters for serial

public:

    void initSizeOfGrid();

    void initMaxIter();

    void compareOrNot();

    void numOfthreads();

    void back_page();

    void pureRunParallel();

    void Compare();

    static int read_row(const char *filename);

    void initLoad();

    static vector<vector<bool>> readFile(int matRow, const char *fileName);

    Serial serial;
    Parallel parallel;
};

