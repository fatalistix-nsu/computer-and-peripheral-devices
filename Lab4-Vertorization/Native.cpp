#include <iomanip>
#include <iostream>
#include <random>
#include <ctime>

using namespace std;

static int MATRIX_SIZE = 2048;
static int NUM_OF_ITERATIONS = 10;
static int MAX_MATRIX_VALUE = 10;

inline void MakeIdentityMatrix(float * Matrix)
{
    fill(Matrix, Matrix + (MATRIX_SIZE * MATRIX_SIZE), 0);
    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        Matrix[i * MATRIX_SIZE + i] = 1.;
    }
}

inline void TransposeMatrix(const float * Matrix, float * Result)
{
    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        for (int j = 0; j < MATRIX_SIZE; j++)
        {
            Result[i * MATRIX_SIZE + j] = Matrix[j * MATRIX_SIZE + i];
        }
    }
}

inline float MaxAbsRawSum(float * Matrix)
{
    float forRet = Matrix[0];
    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        float sum = 0;
        for (int j = 0; j < MATRIX_SIZE; j++)
        {
            sum += abs(Matrix[i * MATRIX_SIZE + j]);
        }
        forRet = max(sum, forRet);
    }
    return forRet;
}

inline void MulMatrixWithScalar(const float * Matrix1, const float Scalar, float * Result)
{
    for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++)
    {
        Result[i] = Matrix1[i] * Scalar;
    }
}

inline void AddMatrices(const float * Matrix1, const float * Matrix2, float * Result)
{
    for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++)
    {
        Result[i] = Matrix1[i] + Matrix2[i];
    }
}

inline void SubMatrices(const float * Matrix1, const float * Matrix2, float * Result)
{
    for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++)
    {
        Result[i] = Matrix1[i] - Matrix2[i];
    }
}

inline void MulMatrices(const float * Matrix1, const float * Matrix2, float * Result)
{
    fill(Result, Result + (MATRIX_SIZE * MATRIX_SIZE), 0);
    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        for (int k = 0; k < MATRIX_SIZE; k++)
        {
            for (int j = 0; j < MATRIX_SIZE; j++)
            {
                Result[i * MATRIX_SIZE + j] += Matrix1[i * MATRIX_SIZE + k] * Matrix2[k * MATRIX_SIZE + j];
            }
        }
    }
}

inline void PrintMatrix(const float * Matrix)
{
    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        for (int j = 0; j < MATRIX_SIZE; j++)
        {
            cout << left << setw(15) << Matrix[i * MATRIX_SIZE + j] << ' ';
        }
        cout << endl;
    }
    cout << endl;
}

inline float * FindInverse(float * A)
{
    auto * I   = new float[MATRIX_SIZE * MATRIX_SIZE];
    auto * A_T = new float[MATRIX_SIZE * MATRIX_SIZE];
    auto * B   = new float[MATRIX_SIZE * MATRIX_SIZE];
    auto * R   = new float[MATRIX_SIZE * MATRIX_SIZE];
    auto * Rn  = new float[MATRIX_SIZE * MATRIX_SIZE];
    auto * Res = new float[MATRIX_SIZE * MATRIX_SIZE];
    auto * buf = new float[MATRIX_SIZE * MATRIX_SIZE];

    // Make I
    MakeIdentityMatrix(I);

    // Make A_T
    TransposeMatrix(A, A_T);

    // Count constants
    float a_1   = MaxAbsRawSum(A_T);
    float a_inf = MaxAbsRawSum(A);

    // Make B
    MulMatrixWithScalar(A_T, 1 / (a_1 * a_inf), B);

    // FIll R
    MulMatrices(B, A, buf);
    SubMatrices(I, buf, R);

    // Make base of Result
    copy(I, I + (MATRIX_SIZE * MATRIX_SIZE), Res);

    // FIll Rn
    copy(R, R + (MATRIX_SIZE * MATRIX_SIZE), Rn);

    for (size_t i = 0; i < NUM_OF_ITERATIONS; i++)
    {
        AddMatrices(Res, Rn, buf);    // Adds Rn to Res and saves it to buf
        swap(buf, Res);                              // swaps previous value with new in buf
        MulMatrices(Rn, R, buf);      // Makes Rn --> Rn+1 and saves to buf
        swap(buf, Rn);                               // Swaps previous value of Rn and new in buf
    }

    MulMatrices(Res, B, buf); // Multiplies Res and buf and saves result to buf
    swap(Res, buf);                          // Load result to Res from buf and saves previous in buf

    delete[] I;
    delete[] A_T;
    delete[] B;
    delete[] R;
    delete[] Rn;
    delete[] buf;

    return Res;
}

int main(int argc, char * argv[])
{
    auto * A   = new float[MATRIX_SIZE * MATRIX_SIZE];

    // Fill A
    for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++)
    {
        A[i] = static_cast<float> (random() % MAX_MATRIX_VALUE);
    }

    if (argc > 1)
    {
        MATRIX_SIZE = stoi(argv[1]);
        if (argc > 2)
        {
            NUM_OF_ITERATIONS = stoi(argv[2]);
        }
    }

    clock_t start = clock();
    auto * Result = FindInverse(A);

//    PrintMatrix(Result);

    clock_t final = clock();

//    auto * buf = new float[MATRIX_SIZE * MATRIX_SIZE];

//    MulMatrices(A, Result, buf);
//    PrintMatrix(buf);

    cout << (double(final - start)) / CLOCKS_PER_SEC << endl;

    delete[] A;
//    delete[] buf;
    delete[] Result;

    return 0;
}
