#include <iostream>
#include <fstream>
#include <math.h>
#include <string>

using namespace std;
double computeMetallicMean(double n)
{   
    double result;

    result = pow(n, 2);
    result += 4;
    result = sqrt(result);
    result += n;
    result /= 2;

    return result;

}

int main()
{
     cerr << "The first ten metallic means:" << endl;
     for (int i = 0; i < 10; i++)
     {
         cout << "n= " << i << "    Value: " << computeMetallicMean(i) << endl;
     }
}