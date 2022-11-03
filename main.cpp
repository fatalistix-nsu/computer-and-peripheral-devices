#include <cmath>
#include <ctime>
#include <iostream>

const double pi = 3.14159265358979323846;

using namespace std;

long double power(long double x, size_t n)
{
    long double forRet = 1;
    for (size_t i = 1; i <= n; i++)
    {
        forRet *= x;
    }

    return forRet;
}

long double factorial(size_t n)
{
    long double forRet = 1;
    for (size_t i = 1; i <= n; i++)
    {
        forRet *= i;
    }

    return forRet;
}

long double Sin(long double x, size_t n)
{
    int sign = (x < 0) ? -1 : 1;
    x = fmod(fabs(x),2 * pi);
    if (x > pi)
    {
        x -= pi;
        sign *= -1;
    }
    if (x > pi/2)
    {
        x = pi - x;
    }

    long double result = 0;
    for (size_t i = 1; i <= n; i++)
    {
        long double buf = power(-1, i - 1) * power(x, 2 * i - 1) / factorial(2 * i - 1);
        if (isnan(buf))
        {
            result += 0;
        }
        else
        {
            result += buf;
        }
    }

    return result * sign;
}

int main(int argc, char* argv[])
{
    size_t numberOfElements = stoull(argv[1]);
    long double x = 10;

    cout << fixed << dec << "Answer: " << Sin(x, numberOfElements) << endl;
    cout << "Time: " << (long double)(clock()) / CLOCKS_PER_SEC << "s" << endl;

    return 0;
}