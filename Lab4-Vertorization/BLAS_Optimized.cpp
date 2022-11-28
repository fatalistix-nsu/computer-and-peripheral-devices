#include <iomanip>
#include <iostream>
#include <random>
#include <cblas.h>

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
        forRet = max(cblas_sasum(MATRIX_SIZE, Matrix + i * MATRIX_SIZE, 1), forRet);
    }
    return forRet;
}

inline void MulMatrixWithScalar(float * Matrix, const float Scalar)
{
    cblas_sscal(MATRIX_SIZE * MATRIX_SIZE, Scalar, Matrix, 1);
}

inline void AddMatrices(float * Matrix1, float * Matrix2)
{
    cblas_saxpy(MATRIX_SIZE * MATRIX_SIZE, 1, Matrix1, 1, Matrix2, 1);
}

inline void MulMatrices(const float * Matrix1, const float * Matrix2, float * Result)
{
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, MATRIX_SIZE, MATRIX_SIZE,
                MATRIX_SIZE, 1.0, Matrix1, MATRIX_SIZE, Matrix2, MATRIX_SIZE, 0.0, Result, MATRIX_SIZE);
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
    auto * B   = new float[MATRIX_SIZE * MATRIX_SIZE];
    auto * R   = new float[MATRIX_SIZE * MATRIX_SIZE];
    auto * buf = new float[MATRIX_SIZE * MATRIX_SIZE];
    auto * Res = new float[MATRIX_SIZE * MATRIX_SIZE];
    auto * Rn  = new float[MATRIX_SIZE * MATRIX_SIZE];

    MakeIdentityMatrix(I);
    TransposeMatrix(A, B);

    float a_1   = MaxAbsRawSum(B);
    float a_inf = MaxAbsRawSum(A);

    copy(I, I + MATRIX_SIZE * MATRIX_SIZE, R);
    MulMatrixWithScalar(B, 1 / a_1 / a_inf);
    MulMatrices(B, A, buf);
    MulMatrixWithScalar(buf, -1);
    AddMatrices(R, buf);

    swap(buf, R);

    // Make base of Result
    copy(I, I + (MATRIX_SIZE * MATRIX_SIZE), Res);

    // FIll Rn
    copy(R, R + (MATRIX_SIZE * MATRIX_SIZE), Rn);

    for (size_t i = 0; i < NUM_OF_ITERATIONS; i++)
    {
        AddMatrices(Rn, Res);
        MulMatrices(Rn, R, buf);
        swap(buf, Rn);
    }

    MulMatrices(Res, B, buf);
    swap(Res, buf);

    delete[] I;
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
