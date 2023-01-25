#pragma once

#include <iomanip>
#include <iostream>
#include <random>
#include <set>
#include <vector>


inline void GenerateDirectBypass(int * array, size_t size)
{
    for (int i = 0; i < size - 1; i++) array[i] = i + 1;
    array[size - 1] = 0;
}

inline void GenerateReverseBypass(int * array, size_t size)
{
    for (int i = 1; i < size; i++) array[i] = i - 1;
    array[0] = int(size - 1);
}

inline bool CheckPermutation(const int * array, size_t size, bool print = true)
{
    if (print)
    {
        std::cout << std::left << std::setw(7) << 0 << ' ';
    }
    int k = 0;
    k = array[k];
    size_t cnt = 0;
    while (k)
    {
        if (print)
        {
            std::cout << std::left << std::setw(7) << k << ' ';
        }
        k = array[k];
        cnt++;
    }
    if (print)
    {
        std::cout << "\nPermutation status (true = correct): ";
        std::cout << std::boolalpha << (cnt == size - 1) << std::endl;
    }
    return cnt == size - 1;
}


inline void GenerateRandomBypass(int * array, size_t size)
{
    std::set <int> set;
    int j;
    set.insert(j = 0);
    for (int i = 0; i < size - 1; i++)
    {
        int k;
        while (set.contains(k = static_cast <int> (random() % size)));
        set.insert(k);
        array[j] = k;
        j = k;
    }
    array[j] = 0;
}

inline void GenerateRandomBypassLineal(int * array, size_t previousSize, size_t newSize)
{
    while (previousSize < newSize)
    {
        size_t index = random() % previousSize;
        array[previousSize]  = array[index];
        array[index] = static_cast<int> (previousSize);
        ++previousSize;
    }
}
