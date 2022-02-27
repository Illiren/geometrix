#pragma once
/*
 * File just contains Intrinsic and fallback implementations of basic matrix
 * operations for matrix floating-point class specializations
*/

#include "immintrin.h"
#include <type_traits>


namespace _Impl
{
// =================================== 4D Vector============================//
template<typename T>
void subVecFallbackImplementation(T (&a)[4], T (&b)[4], T (&result)[4]) requires(std::is_floating_point_v<T>)
{
    result[0] =  a[0]-b[0];
    result[1] =  a[1]-b[1];
    result[2] =  a[2]-b[2];
    result[3] =  a[3]-b[3];
}
template<typename T>
void addVecFallbackImplementation(T (&a)[4], T (&b)[4], T (&result)[4]) requires(std::is_floating_point_v<T>)
{
    result[0] =  a[0]+b[0];
    result[1] =  a[1]+b[1];
    result[2] =  a[2]+b[2];
    result[3] =  a[3]+b[3];
}
template<typename T>
void mulVecFallbackImplementation(T (&a)[4], T (&b)[4], T (&result)[4]) requires(std::is_floating_point_v<T>)
{
    result[0] =  a[0]*b[0];
    result[1] =  a[1]*b[1];
    result[2] =  a[2]*b[2];
    result[3] =  a[3]*b[3];
}
template<typename T>
void divVecFallbackImplementation(T (&a)[4], T (&b)[4], T (&result)[4]) requires(std::is_floating_point_v<T>)
{
    result[0] =  a[0]/b[0];
    result[1] =  a[1]/b[1];
    result[2] =  a[2]/b[2];
    result[3] =  a[3]/b[3];
}
template<typename T>
void subVecSingleFallbackImplementation(T (&a)[4], T b, T (&result)[4]) requires(std::is_floating_point_v<T>)
{
    result[0] =  a[0]-b;
    result[1] =  a[1]-b;
    result[2] =  a[2]-b;
    result[3] =  a[3]-b;
}
template<typename T>
void addVecSingleFallbackImplementation(T (&a)[4], T b, T (&result)[4]) requires(std::is_floating_point_v<T>)
{
    result[0] =  a[0]+b;
    result[1] =  a[1]+b;
    result[2] =  a[2]+b;
    result[3] =  a[3]+b;
}
template<typename T>
void mulVecSingleFallbackImplementation(T (&a)[4], T b, T (&result)[4]) requires(std::is_floating_point_v<T>)
{
    result[0] =  a[0]*b;
    result[1] =  a[1]*b;
    result[2] =  a[2]*b;
    result[3] =  a[3]*b;
}
template<typename T>
void divVecSingleFallbackImplementation(T (&a)[4], T b, T (&result)[4]) requires(std::is_floating_point_v<T>)
{
    result[0] =  a[0]/b;
    result[1] =  a[1]/b;
    result[2] =  a[2]/b;
    result[3] =  a[3]/b;
}

#ifdef __SSE__
void subVecIntrinImplementation(float (&a)[4], float (&b)[4], float (&result)[4])
{
    _mm_store_ps(result, _mm_sub_ps(_mm_load_ps(a), _mm_load_ps(b)));
}
void addVecIntrinImplementation(float (&a)[4], float (&b)[4], float (&result)[4])
{
    _mm_store_ps(result, _mm_add_ps(_mm_load_ps(a), _mm_load_ps(b)));
}

void mulVecIntrinImplementation(float (&a)[4], float (&b)[4], float (&result)[4])
{
    _mm_store_ps(result, _mm_mul_ps(_mm_load_ps(a), _mm_load_ps(b)));
}

void divVecIntrinImplementation(float (&a)[4], float (&b)[4], float (&result)[4])
{
    _mm_store_ps(result, _mm_div_ps(_mm_load_ps(a), _mm_load_ps(b)));
}

void subVecSingleIntrinImplementation(float (&a)[4], float b, float (&result)[4])
{
    _mm_store_ps(result, _mm_sub_ps(_mm_load_ps(a), _mm_set1_ps(b)));
}

void addVecSingleIntrinImplementation(float (&a)[4], float b, float (&result)[4])
{
    _mm_store_ps(result, _mm_add_ps(_mm_load_ps(a), _mm_set1_ps(b)));
}


void mulVecSingleIntrinImplementation(float (&a)[4], float b, float (&result)[4])
{
    _mm_store_ps(result, _mm_mul_ps(_mm_load_ps(a), _mm_set1_ps(b)));
}

void divVecSingleIntrinImplementation(float (&a)[4], float b, float (&result)[4])
{
    _mm_store_ps(result, _mm_div_ps(_mm_load_ps(a), _mm_set1_ps(b)));
}
#endif

// double versions
#ifdef __AVX__
void subVecIntrinImplementation(double (&a)[4], double (&b)[4], double (&result)[4])
{
    _mm256_store_pd(result, _mm256_sub_pd(_mm256_load_pd(a), _mm256_load_pd(b)));
}
void addVecIntrinImplementation(double (&a)[4], double (&b)[4], double (&result)[4])
{
    _mm256_store_pd(result, _mm256_add_pd(_mm256_load_pd(a), _mm256_load_pd(b)));
}

void mulVecIntrinImplementation(double (&a)[4], double (&b)[4], double (&result)[4])
{
    _mm256_store_pd(result, _mm256_mul_pd(_mm256_load_pd(a), _mm256_load_pd(b)));
}

void divVecIntrinImplementation(double (&a)[4], double (&b)[4], double (&result)[4])
{
    _mm256_store_pd(result, _mm256_div_pd(_mm256_load_pd(a), _mm256_load_pd(b)));
}

void subVecSingleIntrinImplementation(double (&a)[4], double b, double (&result)[4])
{
    _mm256_store_pd(result, _mm256_sub_pd(_mm256_load_pd(a), _mm256_set1_pd(b)));
}

void addVecSingleIntrinImplementation(double (&a)[4], double b, double (&result)[4])
{
    _mm256_store_pd(result, _mm256_add_pd(_mm256_load_pd(a), _mm256_set1_pd(b)));
}

void mulVecSingleIntrinImplementation(double (&a)[4], double b, double (&result)[4])
{
    _mm256_store_pd(result, _mm256_mul_pd(_mm256_load_pd(a), _mm256_set1_pd(b)));
}

void divVecSingleIntrinImplementation(double (&a)[4], double b, double (&result)[4])
{
    _mm256_store_pd(result, _mm256_div_pd(_mm256_load_pd(a), _mm256_set1_pd(b)));
}
#endif


// =================================== 4x4 Matrix ============================//
void subMatrixFallbackImplementation(float (&a)[4][4], float (&b)[4][4], float (&result)[4][4])
{
    for (int i = 4; i--;)
        for (int j = 4; j--; a[i][j] -= b[i][j]); // a is the same address as result
}

void addMatrixFallbackImplementation(float (&a)[4][4], float (&b)[4][4], float (&result)[4][4])
{
    for (int i = 4; i--;)
        for (int j = 4; j--; a[i][j] += b[i][j]);
}

void mulMatrixFallbackImplementation(float (&a)[4][4], float (&b)[4][4], float (&result)[4][4])
{
    for (int i = 4; i--;)
        for (int j = 4; j--; a[i][j] *= b[i][j]);
}

void divMatrixFallbackImplementation(float (&a)[4][4], float (&b)[4][4], float (&result)[4][4])
{
    for (int i = 4; i--;)
        for (int j = 4; j--; a[i][j] /= b[i][j]);
}

void subMatrixSingleFallbackImplementation(float (&a)[4][4], float b, float (&result)[4][4])
{
    for (int i = 4; i--;)
        for (int j = 4; j--; a[i][j] -= b);
}

void addMatrixSingleFallbackImplementation(float (&a)[4][4], float b, float (&result)[4][4])
{
    for (int i = 4; i--;)
        for (int j = 4; j--; a[i][j] += b);
}

void mulMatrixSingleFallbackImplementation(float (&a)[4][4], float b, float (&result)[4][4])
{
    for (int i = 4; i--;)
        for (int j = 4; j--; a[i][j] *= b);
}

void divMatrixSingleFallbackImplementation(float (&a)[4][4], float b, float (&result)[4][4])
{
    for (int i = 4; i--;)
        for (int j = 4; j--; a[i][j] /= b);
}

#ifdef __AVX512F__
void subMatrixIntrinImplementation(float (&a)[4][4], float (&b)[4][4], float (&result)[4][4])
{
     _mm512_store_ps(result, _mm512_sub_ps(_mm512_load_ps(a), _mm512_load_ps(b)));
}

void addMatrixIntrinImplementation(float (&a)[4][4], float (&b)[4][4], float (&result)[4][4])
{
     _mm512_store_ps(result, _mm512_add_ps(_mm512_load_ps(a), _mm512_load_ps(b)));
}

void mulMatrixIntrinImplementation(float (&a)[4][4], float (&b)[4][4], float (&result)[4][4])
{
     _mm512_store_ps(result, _mm512_mul_ps(_mm512_load_ps(a), _mm512_load_ps(b)));
}

void divMatrixIntrinImplementation(float (&a)[4][4], float (&b)[4][4], float (&result)[4][4])
{
     _mm512_store_ps(result, _mm512_div_ps(_mm512_load_ps(a), _mm512_load_ps(b)));
}

void subMatrixSingleIntrinImplementation(float (&a)[4][4], float b, float (&result)[4][4])
{
    _mm512_store_ps(result, _mm512_sub_ps(_mm512_load_ps(a), _mm512_set1_ps(b)));
}

void addMatrixSingleIntrinImplementation(float (&a)[4][4], float b, float (&result)[4][4])
{
    _mm512_store_ps(result, _mm512_add_ps(_mm512_load_ps(a), _mm512_set1_ps(b)));
}

void mulMatrixSingleIntrinImplementation(float (&a)[4][4], float b, float (&result)[4][4])
{
    _mm512_store_ps(result, _mm512_mul_ps(_mm512_load_ps(a), _mm512_set1_ps(b)));
}

void divMatrixSingleIntrinImplementation(float (&a)[4][4], float b, float (&result)[4][4])
{
    _mm512_store_ps(result, _mm512_div_ps(_mm512_load_ps(a), _mm512_set1_ps(b)));
}
#endif

}

/*
float (&subFallbackImplementation(float (&a)[4], float (&b)[4]))[4]
{
    float result[4]  {a[0]-b[0],
                      a[1]-b[1],
                      a[2]-b[2],
                      a[3]-b[3]};
    return result;
}*/

/* std::array<float,4> subFallbackImplementation(float (&a)[4], float (&b)[4])
{
    std::array<float,4> result {a[0]-b[0],
                                a[1]-b[1],
                                a[2]-b[2],
                                a[3]-b[3]};
    return result;
}*/
