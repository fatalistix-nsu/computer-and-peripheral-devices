#include <iomanip>
#include <iostream>
#include <x86intrin.h>

#include "Function__GenerateArray.h"

using namespace std;

constexpr size_t START_SIZE = 100;
constexpr size_t FINAL_SIZE = 5e6;
constexpr size_t NUMBER_OF_ROUNDS = 5;
constexpr double ratio = 1.1;

static int buf = 0;

long double func_AverageBypassTick(const int * array, size_t size)
{
//    int k = reversed ? int(size - 1) : 0;
    int k = 0;
    for (int i = 0; i < size * NUMBER_OF_ROUNDS; i++)
    {
        k = array[k];
        if (k == 4) buf = 7;
    }

    long double minTime = UINT32_MAX;

    for (int j = 0; j < 6; j++)
    {
        k = 0;
        size_t start = __rdtsc();
        for (int i = 0; i < size * NUMBER_OF_ROUNDS; i++) k = array[k];
        size_t end = __rdtsc();
        if (k == 4) buf = k;
        minTime = min(minTime, static_cast <long double> (end - start) / (size * NUMBER_OF_ROUNDS));
    }
    return minTime;
}

int main(int argc, char * argv[])
{
    auto * array = new int[FINAL_SIZE];
    size_t current = START_SIZE;


    while (current <= FINAL_SIZE)
    {
        GenerateDirectBypass(array, current);
        cout << setw(7) << left << func_AverageBypassTick(array, current) << "; "
        << (long double) (current) * sizeof(int) / 1024 << endl;
        current = static_cast <size_t> (static_cast <double> (current) * ratio);
    }

    delete[] array;
    cerr << buf;
	return 0;
}
