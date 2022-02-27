#pragma once
/*================ Intrinsics for single and double precision ================//
 * File contains implementations of sin, cos and sinCos functions, using SSE+FMA
 * instructions.
 * Should be more accurate because fma does only one rounding instead of two,
 * for each operation.
*/

#include "trigonometry_const.hpp"
#include "immintrin.h"
#include <cmath>
#include <cassert>

using namespace _ConstInternal;


namespace _Impl
{
using namespace Geometrix;
template <typename T>
void sinCosFallbackImplementation(T x, T& s, T& c)
{
    s = std::sin(x);
    c = std::cos(x);
}

#if defined(__SSE2__) && defined(__FMA__)
    float inner_poly_intrin(float x, bool sin) noexcept
    {
        // compute polynomial
        const float x2 = x * x;
        __m128 fmaRes, add;
        __m128 argSquared = _mm_set1_ps(x2);
        if (sin)
        {
            fmaRes = _mm_set1_ps(SIN_DEGREE_9[4]);

            add = _mm_set1_ps(SIN_DEGREE_9[3]);
            fmaRes = _mm_fmadd_ps(fmaRes, argSquared, add);

            add = _mm_set1_ps(SIN_DEGREE_9[2]);
            fmaRes = _mm_fmadd_ps(fmaRes, argSquared, add);

            add = _mm_set1_ps(SIN_DEGREE_9[1]);
            fmaRes = _mm_fmadd_ps(fmaRes, argSquared, add);

            add = _mm_set1_ps(SIN_DEGREE_9[0]);
            fmaRes = _mm_fmadd_ps(fmaRes, argSquared, add);

            std::array<float,4> resVector;
            _mm_store1_ps(resVector.data(), fmaRes);
            return resVector[0] * x;
        }
        else
        {
            fmaRes = _mm_set1_ps(COS_DEGREE_10[5]);

            add = _mm_set1_ps(COS_DEGREE_10[4]);
            fmaRes = _mm_fmadd_ps(fmaRes, argSquared, add);

            add = _mm_set1_ps(COS_DEGREE_10[3]);
            fmaRes = _mm_fmadd_ps(fmaRes, argSquared, add);

            add = _mm_set1_ps(COS_DEGREE_10[2]);
            fmaRes = _mm_fmadd_ps(fmaRes, argSquared, add);

            add = _mm_set1_ps(COS_DEGREE_10[1]);
            fmaRes = _mm_fmadd_ps(fmaRes, argSquared, add);

            add = _mm_set1_ps(COS_DEGREE_10[0]);
            fmaRes = _mm_fmadd_ps(fmaRes, argSquared, add);

            std::array<float,4> resVector;
            _mm_store1_ps(resVector.data(), fmaRes);
            return resVector[0];
        }
    }

    double inner_poly_intrin(double x, bool sin) noexcept
    {
        // compute polynomial
        const double x2 = x * x;
        __m128d fmaRes, add;
        __m128d argSquared = _mm_set1_pd(x2);
        if (sin)
        {
            fmaRes = _mm_set1_pd(SIN_DEGREE_17[8]);

            add = _mm_set1_pd(SIN_DEGREE_17[7]);
            fmaRes = _mm_fmadd_pd(fmaRes, argSquared, add);

            add = _mm_set1_pd(SIN_DEGREE_17[6]);
            fmaRes = _mm_fmadd_pd(fmaRes, argSquared, add);

            add = _mm_set1_pd(SIN_DEGREE_17[5]);
            fmaRes = _mm_fmadd_pd(fmaRes, argSquared, add);

            add = _mm_set1_pd(SIN_DEGREE_17[4]);
            fmaRes = _mm_fmadd_pd(fmaRes, argSquared, add);

            add = _mm_set1_pd(SIN_DEGREE_17[3]);
            fmaRes = _mm_fmadd_pd(fmaRes, argSquared, add);

            add = _mm_set1_pd(SIN_DEGREE_17[2]);
            fmaRes = _mm_fmadd_pd(fmaRes, argSquared, add);

            add = _mm_set1_pd(SIN_DEGREE_17[1]);
            fmaRes = _mm_fmadd_pd(fmaRes, argSquared, add);

            add = _mm_set1_pd(SIN_DEGREE_17[0]);
            fmaRes = _mm_fmadd_pd(fmaRes, argSquared, add);

            std::array<double,2> resVector;
            _mm_store1_pd(resVector.data(), fmaRes);
            return resVector[0] * x;
        }
        else
        {
            fmaRes = _mm_set1_pd(COS_DEGREE_18[9]);

            add = _mm_set1_pd(COS_DEGREE_18[8]);
            fmaRes = _mm_fmadd_pd(fmaRes, argSquared, add);

            add = _mm_set1_pd(COS_DEGREE_18[7]);
            fmaRes = _mm_fmadd_pd(fmaRes, argSquared, add);

            add = _mm_set1_pd(COS_DEGREE_18[6]);
            fmaRes = _mm_fmadd_pd(fmaRes, argSquared, add);

            add = _mm_set1_pd(COS_DEGREE_18[5]);
            fmaRes = _mm_fmadd_pd(fmaRes, argSquared, add);

            add = _mm_set1_pd(COS_DEGREE_18[4]);
            fmaRes = _mm_fmadd_pd(fmaRes, argSquared, add);

            add = _mm_set1_pd(COS_DEGREE_18[3]);
            fmaRes = _mm_fmadd_pd(fmaRes, argSquared, add);

            add = _mm_set1_pd(COS_DEGREE_18[2]);
            fmaRes = _mm_fmadd_pd(fmaRes, argSquared, add);

            add = _mm_set1_pd(COS_DEGREE_18[1]);
            fmaRes = _mm_fmadd_pd(fmaRes, argSquared, add);

            add = _mm_set1_pd(COS_DEGREE_18[0]);
            fmaRes = _mm_fmadd_pd(fmaRes, argSquared, add);

            std::array<double,2> resVector;
            _mm_store1_pd(resVector.data(), fmaRes);
            return resVector[0];
        }
    }

    std::array<float,4> inner_sin_cos_intrin(float x) noexcept
    {
        const float x2 = x * x;
        __m128 _fmaRes, _add;
        __m128 _argSquared = _mm_set1_ps(x2);

        _fmaRes = _mm_set_ps(0, 0, COS_DEGREE_10[5], SIN_DEGREE_11[5]);

        _add = _mm_set_ps(0, 0, COS_DEGREE_10[4], SIN_DEGREE_11[4]);
        _fmaRes = _mm_fmadd_ps(_fmaRes, _argSquared, _add);

        _add = _mm_set_ps(0, 0, COS_DEGREE_10[3], SIN_DEGREE_11[3]);
        _fmaRes = _mm_fmadd_ps(_fmaRes, _argSquared, _add);

        _add = _mm_set_ps(0, 0, COS_DEGREE_10[2], SIN_DEGREE_11[2]);
        _fmaRes = _mm_fmadd_ps(_fmaRes, _argSquared, _add);

        _add = _mm_set_ps(0, 0, COS_DEGREE_10[1], SIN_DEGREE_11[1]);
        _fmaRes = _mm_fmadd_ps(_fmaRes, _argSquared, _add);

        _add = _mm_set_ps(0, 0, COS_DEGREE_10[0], SIN_DEGREE_11[0]);
        _fmaRes = _mm_fmadd_ps(_fmaRes, _argSquared, _add);

        _fmaRes = _mm_mul_ss(_fmaRes, _mm_load_ss(&x)); // multiply sin result by x (lower float) and carry over other bits

        std::array<float,4> resVector;
        _mm_store_ps(resVector.data(), _fmaRes);

        return resVector;
    }

    std::array<double,2> inner_sin_cos_intrin(double x) noexcept
    {
        const double x2 = x * x;
        __m128d _fmaRes, _add;
        __m128d _argSquared = _mm_set1_pd(x2);

        _fmaRes = _mm_set_pd(COS_DEGREE_16[8], SIN_DEGREE_17[8]);

        _add = _mm_set_pd(COS_DEGREE_16[7], SIN_DEGREE_17[7]);
        _fmaRes = _mm_fmadd_pd(_fmaRes, _argSquared, _add);

        _add = _mm_set_pd(COS_DEGREE_16[6], SIN_DEGREE_17[6]);
        _fmaRes = _mm_fmadd_pd(_fmaRes, _argSquared, _add);

        _add = _mm_set_pd(COS_DEGREE_16[5], SIN_DEGREE_17[5]);
        _fmaRes = _mm_fmadd_pd(_fmaRes, _argSquared, _add);

        _add = _mm_set_pd(COS_DEGREE_16[4], SIN_DEGREE_17[4]);
        _fmaRes = _mm_fmadd_pd(_fmaRes, _argSquared, _add);

        _add = _mm_set_pd(COS_DEGREE_16[3], SIN_DEGREE_17[3]);
        _fmaRes = _mm_fmadd_pd(_fmaRes, _argSquared, _add);

        _add = _mm_set_pd(COS_DEGREE_16[2], SIN_DEGREE_17[2]);
        _fmaRes = _mm_fmadd_pd(_fmaRes, _argSquared, _add);

        _add = _mm_set_pd(COS_DEGREE_16[1], SIN_DEGREE_17[1]);
        _fmaRes = _mm_fmadd_pd(_fmaRes, _argSquared, _add);

        _add = _mm_set_pd(COS_DEGREE_16[0], SIN_DEGREE_17[0]);
        _fmaRes = _mm_fmadd_pd(_fmaRes, _argSquared, _add);

        _fmaRes = _mm_mul_sd(_fmaRes, _mm_load_sd(&x)); // multiply sin result by x (lower double) and carry over other bits

        std::array<double,2> resVector;
        _mm_store_pd(resVector.data(), _fmaRes);

        return resVector;
    }

    template <typename T>
    T sinFastSSEImplementation(T x) noexcept
    {
        // handle large args - set up control variables
        const ReductionRes res = addRangeReduce(x, HALF_PI, INV_HALF_PI);
        int sign;
        bool useSinCoeffs;
        if (res.noReduciton)
        {
            sign = 1;
            useSinCoeffs = true;
        }
        else
        {
            sign = res.quad >= 0 ? 1 : -1;
            x *= sign;
            switch ((res.quad*sign) & Pi3by2_2Pi)
            {
            case Zero_Pi2:
                useSinCoeffs = true;
                break;
            case Pi2_Pi:
                useSinCoeffs = false;
                break;
            case Pi_Pi3by2:
                useSinCoeffs = true;
                sign = -sign;
                break;
            case Pi3by2_2Pi:
                useSinCoeffs = false;
                sign = -sign;
                break;
            default:
                assert(false && "invalid range");
            }
        }

        if (x == 0)
            return sign * (useSinCoeffs ? 0 : 1);

        return sign * inner_poly_intrin(x, useSinCoeffs);
    }

    template <typename T>
    T cosFastSSEImplementation(T x) requires(std::is_floating_point_v<T>)
    {
        // handle large args - set up control variables
        const ReductionRes res = addRangeReduce(x, HALF_PI, INV_HALF_PI);
        int sign;
        bool useSinCoeffs;
        if (res.noReduciton)
        {
            sign = 1;
            useSinCoeffs = false;
        }
        else
        {
            sign = res.quad >= 0 ? 1 : -1;
            x *= sign;
            switch ((res.quad*sign) & Pi3by2_2Pi)
            {
            case Zero_Pi2:
                useSinCoeffs = false;
                sign = 1;
                break;
            case Pi2_Pi:
                useSinCoeffs = true;
                sign = -1;
                break;
            case Pi_Pi3by2:
                useSinCoeffs = false;
                sign = -1;
                break;
            case Pi3by2_2Pi:
                useSinCoeffs = true;
                sign = 1;
                break;
            default:
                assert(false && "invalid range");
            }
        }

        if (x == 0)
            return sign * (useSinCoeffs ? 0 : 1);

        return sign * inner_poly_intrin(x, useSinCoeffs);
    }

    template <typename T>
    void sinCosSSEImplementation(T x, T& s, T& c)
    {
        // handle large args - set up control variables
        const ReductionRes res = addRangeReduce(x, HALF_PI, INV_HALF_PI);

        // 0 index is for sin and 1 is for cos
        std::array<int,2> sign;
        std::array<int,2> resVectorIndex; // defines which part of the approximation to use (sin or cos) with index in resulting vector
        if (res.noReduciton)
        {
            sign[0] = 1;
            sign[1] = 1;
            resVectorIndex[0] = 0;
            resVectorIndex[1] = 1;
        }
        else
        {
            sign[0] = res.quad >= 0 ? 1 : -1;
            x *= sign[0];
            switch ((res.quad*sign[0]) & Pi3by2_2Pi)
            {
            case Zero_Pi2:
                resVectorIndex[0] = 0;
                resVectorIndex[1] = 1;
                sign[1] = 1;
                break;
            case Pi2_Pi:
                resVectorIndex[0] = 1;
                resVectorIndex[1] = 0;
                sign[1] = -1;
                break;
            case Pi_Pi3by2:
                resVectorIndex[0] = 0;
                sign[0] = -sign[0];
                resVectorIndex[1] = 1;
                sign[1] = -1;
                break;
            case Pi3by2_2Pi:
                resVectorIndex[0] = 1;
                sign[0] = -sign[0];
                resVectorIndex[1] = 0;
                sign[1] = 1;
                break;
            default:
                assert(false && "invalid range");
            }
        }

        if (x == 0)
        {
            s = sign[0] * resVectorIndex[0];
            c = sign[1] * resVectorIndex[1];
            return;
        }

        if constexpr (std::is_same_v<T, float>)
        {
            std::array<float,4> resVector = inner_sin_cos_intrin(x);

            s = sign[0] * resVector[resVectorIndex[0]];
            c = sign[1] * resVector[resVectorIndex[1]];
        }
        else
        {
            std::array<double,2> resVector = inner_sin_cos_intrin(x);

            s = sign[0] * resVector[resVectorIndex[0]];
            c = sign[1] * resVector[resVectorIndex[1]];
        }
    }
#endif

}
