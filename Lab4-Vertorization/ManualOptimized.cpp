#include <iomanip>
#include <iostream>
#include <random>
#include <immintrin.h>

using namespace std;

static int MATRIX_SIZE = 2048;
static int NUM_OF_ITERATIONS = 10;
static int MAX_MATRIX_VALUE = 10;

inline void AVX2_FillZero(float * Matrix)
{
    auto reg0 = _mm256_setzero_ps();
    for (auto i = Matrix; i < Matrix + MATRIX_SIZE * MATRIX_SIZE; i += 8)
    {
        _mm256_store_ps(i, reg0);
    }
}

inline void AVX2_MakeIdentityMatrix(float * Matrix)
{
    AVX2_FillZero(Matrix);
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

//inline float AVX2_MaxAbsRawSum(const float * Matrix)
//{
//    auto * absMask = new int[8]{INT32_MAX, INT32_MAX, INT32_MAX, INT32_MAX,
//                                INT32_MAX, INT32_MAX, INT32_MAX, INT32_MAX};
//
//    auto reg0 = _mm256_load_ps((float*)absMask);
//    delete[] absMask;
//    float forRet = Matrix[0];
//
//    float * buf = new float[20]{0, 0, 0, 0, 0, 0, 0, 0};
//    for (int i = 0; i < MATRIX_SIZE; i++)
//    {
//        __m256 reg1 = _mm256_setzero_ps();
//        float forCompare = 0;
//        for (auto currentRaw = Matrix + i * MATRIX_SIZE; currentRaw < Matrix + (i + 1) * MATRIX_SIZE; currentRaw += 8)
//        {
//            auto reg2 = _mm256_load_ps(currentRaw);
//            reg2 = _mm256_and_ps(reg2, reg0);
//            reg1 = _mm256_add_ps(reg1, reg2);
//        }
//        std::cout << i << std::endl;
//        _mm256_store_ps(buf, reg1);
//        std::cout << i << std::endl;
//        for (int c = 0; c < 8; c++)
//        {
//            forCompare += buf[c];
//        }
//        forRet = max(forCompare, forRet);
//    }
//    delete[] buf;
//    return forRet;
//}

inline void AVX2_MulMatrixWithScalar(const float * Matrix, const float scalar, float * Result)
{
    auto * scalarVector = new float[8];
    for (int i = 0; i < 8; i++)
    {
        scalarVector[i] = scalar;
    }

    auto reg0 = _mm256_load_ps(scalarVector);

    delete[] scalarVector;


    for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i+= 8)
    {
        auto reg1 = _mm256_load_ps(Matrix + i);
        reg1 = _mm256_mul_ps(reg1, reg0);
        _mm256_store_ps(Result + i, reg1);
    }
}

inline void AVX2_AddMatrices(const float * A, const float * B, float * Res)
{
    for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i+=8)
    {
        auto reg0 = _mm256_load_ps(A + i);
        auto reg1 = _mm256_load_ps(B + i);

        reg0 = _mm256_add_ps(reg0, reg1);
        _mm256_store_ps(Res + i, reg0);
    }
}

inline void AVX2_SubMatrices(const float * A, const float * B, float * Res)
{
    for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i+=8)
    {
        auto reg0 = _mm256_load_ps(A + i);
        auto reg1 = _mm256_load_ps(B + i);

        reg0 = _mm256_sub_ps(reg0, reg1);
        _mm256_store_ps(Res + i, reg0);
    }
}

inline void AVX2_MulMatrices(const float * A, const float * B, float * Res)
{
    AVX2_FillZero(Res);
    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        for (int k = 0; k < MATRIX_SIZE; k++)
        {
            float buf[8];
            std::fill(buf, buf + 8, A[i * MATRIX_SIZE + k]);
            auto reg0 = _mm256_load_ps(buf);
            for (int j = 0; j < MATRIX_SIZE; j+=8)
            {
                auto reg1 = _mm256_load_ps(B + (k * MATRIX_SIZE) + j);
                auto reg2 = _mm256_load_ps(Res + (i * MATRIX_SIZE) + j);
                reg1 = _mm256_mul_ps(reg0, reg1);
                reg2 = _mm256_add_ps(reg2, reg1);
                _mm256_store_ps(Res + (i * MATRIX_SIZE) + j, reg2);
            }
        }
    }
}

inline void AVX2_CopyMatrix(const float * A, float * Res)
{
    for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i+=8)
    {
        auto reg0 = _mm256_load_ps(A + i);
        _mm256_store_ps(Res + i, reg0);
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
    AVX2_MakeIdentityMatrix(I);

    // Make A_T
    TransposeMatrix(A, A_T);

    // Count constants
    float a_1   = MaxAbsRawSum(A_T);
    float a_inf = MaxAbsRawSum(A);

    // Make B
    AVX2_MulMatrixWithScalar(A_T, 1 / (a_1 * a_inf), B);

    // FIll R
    AVX2_MulMatrices(B, A, buf);
    AVX2_SubMatrices(I, buf, R);

    // Make base of Result
    AVX2_CopyMatrix(I, Res);

    // FIll Rn
    AVX2_CopyMatrix(R, Rn);

    for (size_t i = 0; i < NUM_OF_ITERATIONS; i++)
    {
        AVX2_AddMatrices(Res, Rn, buf);    // Adds Rn to Res and saves it to buf
        swap(buf, Res);                              // swaps previous value with new in buf
        AVX2_MulMatrices(Rn, R, buf);      // Makes Rn --> Rn+1 and saves to buf
        swap(buf, Rn);                               // Swaps previous value of Rn and new in buf
    }

    AVX2_MulMatrices(Res, B, buf); // Multiplies Res and buf and saves result to buf
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

//    AVX2_MulMatrices(A, Result, buf);
//    PrintMatrix(buf);

    cout << (double(final - start)) / CLOCKS_PER_SEC << endl;

    delete[] A;
//    delete[] buf;
    delete[] Result;

    return 0;
}


