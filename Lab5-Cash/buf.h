union ticks{
    unsigned long long t64;
    struct s32 { long th, tl; } t32;
};

long double asm_AverageBypassTick(const int * array, size_t size, bool reversed = false)
{
    int k = reversed ? int(size - 1) : 0;
    for (int i = 0; i < size; i++)
    {
        k = array[k];
        if (k == 4) buf = k;
    }

    ticks start{}, end{};
    asm("rdtsc\n":"=a"(start.t32.th),"=d"(start.t32.tl));
    for (int i = 0; i < size; i++) k = array[k];
    asm("rdtsc\n":"=a"(end.t32.th),"=d"(end.t32.tl));
    if (k == 7) buf = k;
    return static_cast <long double> (end.t64 - start.t64) / size;
}