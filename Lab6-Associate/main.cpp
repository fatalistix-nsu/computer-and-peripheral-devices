#include <iostream>
#include <x86intrin.h>

#include "MulMatrices.h"

constexpr size_t LEVEL1_BANK_SIZE = 32  * 1024 / 8;
constexpr size_t LEVEL2_BANK_SIZE = 512 * 1024 / 8;
constexpr size_t LEVEL3_BANK_SIZE = 4 * 1024 * 1024 / 16;
constexpr size_t CACHE_LINE_SIZE = 64;
constexpr size_t OFFSET = 16 * 1024 * 1024;
constexpr size_t MAX_NUM_OF_FRAGMENTS = 100;
constexpr size_t SIZE_T_OFFSET = OFFSET / sizeof(size_t);
constexpr size_t NUM_OF_ITERATIONS = 10;
constexpr size_t SIZE_OF_CHECKING_ARRAY = 64;

static size_t buf = 0;

inline void FillArray(size_t * array, size_t size)
{
    for (size_t i = 0; i < size - SIZE_T_OFFSET; i += SIZE_T_OFFSET)
    {
        for (size_t j = 0; j < SIZE_OF_CHECKING_ARRAY; j++)
        {
            array[i + j] = i + j + SIZE_T_OFFSET;
        }
    }
    for (size_t j = 0; j < SIZE_OF_CHECKING_ARRAY; j++)
    {
        array[size - SIZE_T_OFFSET + j] = (j + 1) % SIZE_OF_CHECKING_ARRAY;
    }
}

int main()
{
    std::cerr << sizeof(size_t) << std::endl;
    auto * matrix1 = new float[1024 * 1024];
    auto * matrix2 = new float[1024 * 1024];
    auto * matrix3 = new float[1024 * 1024];
    for (size_t i = 0; i < 10; i++)
    {
        AVX2_MulMatrices(matrix1, matrix2, matrix3, 1024);
    }

    auto * array = new size_t[MAX_NUM_OF_FRAGMENTS * SIZE_T_OFFSET];


    for (size_t i = 1; i <= MAX_NUM_OF_FRAGMENTS; i++)
    {
        FillArray(array, i * SIZE_T_OFFSET);

        long double result = UINT64_MAX;

        for (size_t j = 0; j < 6; j++)
        {
            size_t k = 0, counter = 0;
            size_t start = __rdtsc();
            while (counter < NUM_OF_ITERATIONS)
            {
                k = array[k];
                counter += !k;
            }
            size_t end = __rdtsc();
            buf = counter / i;
            result = std::min(result, static_cast <long double> (end - start) /
            (static_cast<long double> (SIZE_OF_CHECKING_ARRAY) * i * NUM_OF_ITERATIONS));
            std::cerr << buf;
        }
        std::cout << i << "; " << result << std::endl;
    }
    std::cerr << std::endl;



    delete[] array;

    delete[] matrix1;
    delete[] matrix2;
    delete[] matrix3;
    return 0;
}
