#pragma once
#include <x86intrin.h>

inline void AVX2_FillZero(float * Matrix, const size_t size)
{
    auto reg0 = _mm256_setzero_ps();
    for (auto i = Matrix; i < Matrix + size * size; i += 8)
    {
        _mm256_store_ps(i, reg0);
    }
}

inline void AVX2_MulMatrices(const float * A, const float * B, float * Res, const size_t size)
{
    AVX2_FillZero(Res, size);
    for (int i = 0; i < size; i++)
    {
        for (int k = 0; k < size; k++)
        {
            float buf[8];
            std::fill(buf, buf + 8, A[i * size + k]);
            auto reg0 = _mm256_load_ps(buf);
            for (int j = 0; j < size; j+=8)
            {
                auto reg1 = _mm256_load_ps(B + (k * size) + j);
                auto reg2 = _mm256_load_ps(Res + (i * size) + j);
                reg1 = _mm256_mul_ps(reg0, reg1);
                reg2 = _mm256_add_ps(reg2, reg1);
                _mm256_store_ps(Res + (i * size) + j, reg2);
            }
        }
    }
}