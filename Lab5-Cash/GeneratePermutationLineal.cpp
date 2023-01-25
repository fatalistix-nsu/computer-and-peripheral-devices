#include <ctime>
#include <iostream>

#include "Function__GenerateArray.h"

constexpr size_t START_SIZE = 100;
constexpr size_t FINAL_SIZE = 5e6;
constexpr double ratio = 1.1;

int main()
{
    srandom(time(nullptr));
    auto * array = new int[FINAL_SIZE];

    size_t current = START_SIZE;
    size_t prevSize = 1;
    array[0] = 0;

    while (current <= FINAL_SIZE)
    {
        GenerateRandomBypassLineal(array, prevSize, current);
        if (CheckPermutation(array, current, false))
        {
            for (size_t i = 0; i < current; i++)
            {
                std::cout << array[i] << ' ';
            }
            std::cout << std::endl;
        }
        else
        {
            std::cerr << current << ' ';
        }
        prevSize = current;
        current = static_cast <size_t> (static_cast <double> (current) * ratio);
    }


    delete[] array;
    return 0;
}