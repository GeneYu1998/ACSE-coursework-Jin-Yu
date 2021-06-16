#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <vector>

using namespace std;

void remove_even_ascend(vector<int>& array, vector<int>& array2, int &odd_numbers)
{   
    for (int i = 0; i < 10; i++)
    {   
        if (array[i]%2 == 1)
        {   
            odd_numbers++;
            array2.push_back(array[i]);
        }
    }

    int temp;
    for (int k = 0; k < odd_numbers; k++)
    {
    for (int i = 0; i < odd_numbers - 1; i++)
    {   
        if (array2[i] > array2[i+1])
        {   
            temp = array2[i];
            array2[i] = array2[i+1];
            array2[i+1] = temp;

        }
    }
    }
}

int main()
{
    int value[10] = {9, 8, 7, 6 ,5, 4, 3, 2, 1, 0};
    vector<int> array;
    for (int i = 0; i < 10; i++)
    {
    array.push_back(value[i]);
    }

    cout << "the original array we have: " << endl;
    for (int i = 0; i < 10; i++)
    {
    cout << array[i] << " ";
    }
    cout << endl;
    
    int odd_numbers = 0;
    vector<int> array_new;
    remove_even_ascend(array, array_new, odd_numbers);

    cout << "the new array we have: " << endl;
    for (int i = 0; i < odd_numbers; i++)
    {
    cout << array_new[i] << " ";
    }
    cout << endl;    


}


