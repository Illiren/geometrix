/*
 * File contains interface and internal workings of such functions as: sine,
 * cosine, tangent, arc-sin/cos/tan. Function interface allows for accuracy/speed
 * tradeoff with two implementations: table-based and polinom-based,
 * and also using CPU instructions with fixed accuracy for single and double
 * floating point precision
*/
#pragma once


#include <cmath>
#include <type_traits>
#include <cassert>
#include "trigonometry_const.hpp"
#include "lut_generator.hpp"
#include <limits>
#include <inttypes.h>
#include "optimizer.hpp"

using namespace _ConstInternal;

namespace Geometrix
{
namespace Trig
{
/*
 * Approximation of acos(x) with a rational function f such that the
 * worst absolute error is minimal. That is, pick the function that performs best
 * in the worst case, and with following restrictions: acos(0) = Pi/2, acos(1) = 0, acos(-1) =
 * Source: https://github.com/ruuda/convector/blob/master/tools/approx_acos.py
 * Accuracy: av abs error 3e-11, max abs error 0.0167, accuracy gets worse when aproaching the limits.
 * Up to 2 times faster than std, depending on compiler.
 * Returns the arccosine of x in the range [0,pi], expecting x to be in the range [-1,+1].
*/
template<typename T> constexpr T acos(T x) noexcept requires(std::is_floating_point_v<T>)
{
    assert(x >= -1 && x <= 1 && "invalid argument range");
    constexpr T c1 = -0.939115566365855;
    constexpr T c2 =  0.9217841528914573;
    constexpr T c3 = -1.2845906244690837;
    constexpr T c4 =  0.295624144969963174;
    const T x2 = x * x;
    const T x3 = x * x * x;
    const T x4 = x * x * x * x;

    return HALF_PI + (c1*x + c2*x3) / (1 + c3*x2 + c4*x4);
}

// wrapper function to handle interger arguments
template<typename T> constexpr auto acos(T x) noexcept requires(std::is_integral_v<T>)
{
    return acos<double>(x);
}

// just a shifted version of acos implementation
// Returns the arc cosine of x in the range [-pi/2,pi/2], expecting x to be in the range [-1,+1].
template<typename T> constexpr T asin(T x) noexcept requires(std::is_floating_point_v<T>)
{
    return HALF_PI - acos<T>(x);
}

// wrapper function to handle interger arguments
template<typename T> constexpr auto asin(T x) noexcept requires(std::is_integral_v<T>)
{
    return HALF_PI - acos<double>(x);
}


//=================================== INTERNAL ===============================//
    namespace _Internal
    {
//======================= Generic LU table implementation ====================//

    // loose approximation of what size of the table should be for a given relative error
    constexpr int constLUTSizeFromAcc(double relError, int ratio) noexcept
    {
        return int(M_PI / Trig::acos(1 - relError) / ratio) + 1;
    }
    // maps number of accurate significant needed to the max error value for constLUTSizeFromAcc
    // same values after 5 entry are due to compiler limitation for clang and gcc (at least)
    inline constexpr std::array<double,SIN_COS_ACC_MAP_COUNT> SC_LUT_ACC_MAP =
    { 0.1,0.01,0.001,0.00001,0.00001,0.00001,0.00001,0.00001,0.00001,0.00001,0.00001 };

    // Information for periodic function LUT generation
    template <typename T, T(*Func)(T), std::size_t foldingRatio, std::size_t acc>
    struct LUTInfo
    {
        static constexpr std::size_t size = constLUTSizeFromAcc(SC_LUT_ACC_MAP[acc],foldingRatio);
        static constexpr double startValue = 0;
        static constexpr double endValue = 2*M_PI / foldingRatio;
        static constexpr double step = endValue / size;
        static constexpr auto table = getLUT<T,Func,LUTInfo>;
    };
    /*
     * calculates value with gradient approximation betweeen value points, based
     * on a given TableInfo static struct descibed above
     */
    template <typename T, class TableInfo>
    constexpr T generic_inner_table(T x) noexcept
    {
        x = x / TableInfo::endValue * TableInfo::size;
        auto index = getNearestInt(x);
        if (index == TableInfo::size)
            --index;
        const T diff = x - index;

        const int gradIndex = (diff < 0 && index > 0) ? index-1 : index;

        return std::get<0>(TableInfo::table[index]) + diff * std::get<1>(TableInfo::table[gradIndex]);
    }

//============================= Polynomial implementation ====================//
    template <typename T, std::size_t accuracy>
    constexpr T sin_inner_polinomial(T x) noexcept
    {
        if (x == 0)
            return x;
        const T x2 = x * x;
        constexpr auto polySize = std::get<PolyIndex>(SIN_POLIES[accuracy]);
        T res = std::get<PolyData>(SIN_POLIES[accuracy])[polySize-1];
        for (int i = polySize-2; i >= 0; --i)
            res = res * x2 + std::get<PolyData>(SIN_POLIES[accuracy])[i];

        return res * x;
    }

    template <typename T, std::size_t accuracy>
    constexpr T cos_inner_polinomial(T x) noexcept
    {
        if (x == 0)
            return 1;
        const T x2 = x * x;
        constexpr auto polySize = std::get<PolyIndex>(COS_POLIES[accuracy]);
        T res = std::get<PolyData>(COS_POLIES[accuracy])[polySize-1];
        for (int i = polySize-2; i >= 0; --i)
            res = res * x2 + std::get<PolyData>(COS_POLIES[accuracy])[i];

        return res;
    }

    template <typename T, bool fast>
    constexpr T tan_inner_polynomial(T x) noexcept
    {
        x *= INV_QUARTER_PI;
        const T x2 = x * x;
        if constexpr(fast)
        {
            return x * TAN_DEGREE_2[0] / (TAN_DEGREE_2[1] + x2);
        }
        else
        {
            return x * (TAN_DEGREE_4[0] + TAN_DEGREE_4[1] * x2) /
                    (TAN_DEGREE_4[2] + x2 * (TAN_DEGREE_4[3] + x2));
        }
    }
}
//============================= END INTERNAL =================================//

template <typename T> constexpr T radToDeg(T value) noexcept
{
    return value / DEG_TO_RAD;
}

template <typename T> constexpr T degToRad(T value) noexcept
{
    return value * DEG_TO_RAD;
}
// forward declaractions for LUT generation
template <typename T, std::size_t accuracy = sinCosAcc<T>, bool polyApprox = true>
constexpr T sin(T x) noexcept requires(std::is_floating_point_v<T>);
template <typename T, std::size_t accuracy = sinCosAcc<T>, bool polyApprox = true>
constexpr auto sin(T x) noexcept requires (std::is_integral_v<T>);
//================================== Interface ===============================//
/*
 * Approximations of Sine/Cosine, where:
 * polyApprox - defines implementation, poly for polynomial, else LUT
 * accuracy - integer value in range 0..10 that scales up the accuracy
 * of the approximation, where value stands for number of digits of accurary required
 * in fractional part of the result (i.e. to get 0.0xx accuracy choose accuracy=1,
 * so it guarantees that maximum absolute error will be lower than 0.1 on the argument range).
 * Better accuracy, leads to slower runtime (obviously)
 * By default accuracy is maximum for a given data type and not limited by described digits of accuracy
 * Warning: float versions has lower max accuracy ceiling 1E-7, so the accuracy hold at 0..5 range.
 * Warning2: for LUT implementation accuracy is effective for only 0..3(with 0.00011 max error at 4))
*/
template <typename T, std::size_t accuracy = sinCosAcc<T>, bool polyApprox = true>
constexpr T cos(T x) noexcept requires(std::is_floating_point_v<T>)
{
    static_assert (accuracy < SIN_COS_ACC_MAP_COUNT, "invalid accuracy");
    if (x == std::numeric_limits<T>::infinity()) // don't try to compute inf and signal a nan
        return std::numeric_limits<T>::signaling_NaN();
    const auto range = polyApprox ? HALF_PI : QUARTER_PI;
    const auto invRange = polyApprox ? INV_HALF_PI : INV_QUARTER_PI;
    const ReductionRes res = addRangeReduce(x, range, invRange);
    if constexpr (polyApprox)
    {
        if (res.noReduciton)
            return _Internal::cos_inner_polinomial<T,SIN_COS_ACC_MAP[accuracy]>(x);
        const int sign = res.quad >= 0 ? 1 : -1;
        x *= sign;
        // split function period into 4 equal parts shifted by Pi/2
        switch ((res.quad*sign) & Pi3by2_2Pi)
        {
        case Zero_Pi2:
            return _Internal::cos_inner_polinomial<T,SIN_COS_ACC_MAP[accuracy]>(x);
        case Pi2_Pi:
            return -_Internal::sin_inner_polinomial<T,SIN_COS_ACC_MAP[accuracy]>(x);
        case Pi_Pi3by2:
            return -_Internal::cos_inner_polinomial<T,SIN_COS_ACC_MAP[accuracy]>(x);
        case Pi3by2_2Pi:
            return _Internal::sin_inner_polinomial<T,SIN_COS_ACC_MAP[accuracy]>(x);
        }
    }
    else
    {
        const int sign = x >= 0 ? 1 : -1; // get sign and negate it, so table won't out of range
        x *= sign;
        if (res.noReduciton)
            return _Internal::generic_inner_table<T,_Internal::LUTInfo<double,Trig::cos,SIN_COS_FOLDING_RATIO, accuracy>>(x);
        const int quad = res.quad >= 0 ? res.quad : -res.quad;
        // split function period into 8 equal parts shifted by Pi/4
        switch (quad & PiPi3by4_2Pi)
        {
        case Zero_Pi4:
            return _Internal::generic_inner_table<T,_Internal::LUTInfo<double,Trig::cos,SIN_COS_FOLDING_RATIO, accuracy>>(x);
        case Pi4_Pi2:
            return _Internal::generic_inner_table<T,_Internal::LUTInfo<double,Trig::sin,SIN_COS_FOLDING_RATIO, accuracy>>(QUARTER_PI - x);
        case Pi2_Pi3by4:
            return -_Internal::generic_inner_table<T,_Internal::LUTInfo<double,Trig::sin,SIN_COS_FOLDING_RATIO, accuracy>>(x);
        case Pi3by4_Pi:
            return -_Internal::generic_inner_table<T,_Internal::LUTInfo<double,Trig::cos,SIN_COS_FOLDING_RATIO, accuracy>>(QUARTER_PI - x);
        case PiZero_Pi4:
            return -_Internal::generic_inner_table<T,_Internal::LUTInfo<double,Trig::cos,SIN_COS_FOLDING_RATIO, accuracy>>(x);
        case PiPi4_Pi2:
            return -_Internal::generic_inner_table<T,_Internal::LUTInfo<double,Trig::sin,SIN_COS_FOLDING_RATIO, accuracy>>(QUARTER_PI - x);
        case PiPi2_Pi3by4:
            return _Internal::generic_inner_table<T,_Internal::LUTInfo<double,Trig::sin,SIN_COS_FOLDING_RATIO, accuracy>>(x);
        case PiPi3by4_2Pi:
            return _Internal::generic_inner_table<T,_Internal::LUTInfo<double,Trig::cos,SIN_COS_FOLDING_RATIO, accuracy>>(QUARTER_PI - x);
        }
    }
    assert(false && "invalid range");
    return x;
}

// wrapper function to handle interger arguments
template<typename T, std::size_t accuracy = sinCosAcc<T>, bool polyApprox = true>
constexpr auto cos(T x) noexcept requires (std::is_integral_v<T>)
{
    return cos<double,accuracy,polyApprox>(double(x));
}

template <typename T, std::size_t accuracy, bool polyApprox>
constexpr T sin(T x) noexcept requires(std::is_floating_point_v<T>)
{
    static_assert (accuracy < SIN_COS_ACC_MAP_COUNT, "invalid accuracy");
    if (x == std::numeric_limits<T>::infinity())
        return std::numeric_limits<T>::signaling_NaN();
    if constexpr (polyApprox)
    {
        const ReductionRes res = addRangeReduce(x, HALF_PI, INV_HALF_PI);
        if (res.noReduciton)
            return _Internal::sin_inner_polinomial<T,SIN_COS_ACC_MAP[accuracy]>(x);
        const int sign = res.quad >= 0 ? 1 : -1;
        x *= sign;
        switch ((res.quad*sign) & Pi3by2_2Pi)
        {
        case Zero_Pi2:
            return sign*_Internal::sin_inner_polinomial<T,SIN_COS_ACC_MAP[accuracy]>(x);
        case Pi2_Pi:
            return sign*_Internal::cos_inner_polinomial<T,SIN_COS_ACC_MAP[accuracy]>(x);
        case Pi_Pi3by2:
            return -sign*_Internal::sin_inner_polinomial<T,SIN_COS_ACC_MAP[accuracy]>(x);
        case Pi3by2_2Pi:
            return -sign*_Internal::cos_inner_polinomial<T,SIN_COS_ACC_MAP[accuracy]>(x);
        }
    }
    else
    {
        return cos<T,accuracy,polyApprox>(HALF_PI - x);
    }
    assert(false && "invalid range");
    return x;
}

// wrapper function to handle interger arguments
template <typename T, std::size_t accuracy, bool polyApprox>
constexpr auto sin(T x) noexcept requires (std::is_integral_v<T>)
{
    return sin<double,accuracy,polyApprox>(double(x));
}

template <typename T, std::size_t accuracy = sinCosAcc<T>, bool polyApprox = true>
constexpr T cosDeg(T degrees) noexcept requires(std::is_floating_point_v<T>)
{
    if (degrees == std::numeric_limits<T>::infinity())
        return std::numeric_limits<T>::signaling_NaN();
    return cos<T,accuracy,polyApprox>(degToRad(degrees));
}

template <typename T, std::size_t accuracy = sinCosAcc<T>, bool polyApprox = true>
constexpr T sinDeg(T degrees) noexcept requires(std::is_floating_point_v<T>)
{
    if (degrees == std::numeric_limits<T>::infinity())
        return std::numeric_limits<T>::signaling_NaN();
    return sin<T,accuracy,polyApprox>(degToRad(degrees));
}

// wrapper function to handle interger arguments
template <typename T, std::size_t accuracy = sinCosAcc<T>, bool polyApprox = true>
constexpr auto cosDeg(T degrees) noexcept requires(std::is_integral_v<T>)
{
    return cos<double,accuracy,polyApprox>(degToRad(double(degrees)));
}

// wrapper function to handle interger arguments
template <typename T, std::size_t accuracy = sinCosAcc<T>, bool polyApprox = true>
constexpr auto sinDeg(T degrees) noexcept requires(std::is_integral_v<T>)
{
    return sin<double,accuracy,polyApprox>(degToRad(double(degrees)));
}

/*
 * Polynomial approximation of Tangent(x), where:
 * there are two modes, fast has max relative error of 0.0033 and slow - 1e-7,
 * although runtime is about the same, with slight advantage of the former.
 */
template <typename T, bool fast = true>
constexpr T tan(T x) noexcept requires(std::is_floating_point_v<T>)
{
    const int sign = x >= 0 ? 1 : -1;
    const ReductionRes res = addRangeReduce(x, QUARTER_PI, INV_QUARTER_PI);
    x *= sign;
    if (x == 0 && (res.quad == 2*sign || res.quad == 6*sign))// result approaches infinity for args Pi/2 and 3Pi/2
        return std::numeric_limits<T>::infinity();
    switch ((res.quad*sign) & PiPi3by4_2Pi)
    {
    case Zero_Pi4:
         return sign*_Internal::tan_inner_polynomial<T,fast>(x);
    case Pi4_Pi2:
        return sign/_Internal::tan_inner_polynomial<T,fast>(QUARTER_PI - x);
    case Pi2_Pi3by4:
        return -sign/_Internal::tan_inner_polynomial<T,fast>(x);
    case Pi3by4_Pi:
        return -sign*_Internal::tan_inner_polynomial<T,fast>(QUARTER_PI - x);
    case PiZero_Pi4:
        return sign*_Internal::tan_inner_polynomial<T,fast>(x);
    case PiPi4_Pi2:
        return sign/_Internal::tan_inner_polynomial<T,fast>(QUARTER_PI - x);
    case PiPi2_Pi3by4:
        return -sign/_Internal::tan_inner_polynomial<T,fast>(x);
    case PiPi3by4_2Pi:
        return -sign*_Internal::tan_inner_polynomial<T,fast>(QUARTER_PI - x);
    }
    assert(false && "invalid range");
    return x;
}

// wrapper function to handle interger arguments
template <typename T, bool fast = true>
constexpr auto tan(T x) noexcept requires(std::is_integral_v<T>)
{
    return tan<double,fast>(double(x));
}

template <typename T, bool fast = true>
constexpr auto tanDeg(T degrees) noexcept requires(std::is_floating_point_v<T>)
{
    return tan<T,fast>(degToRad(degrees));
}

// wrapper function to handle interger arguments
template <typename T, bool fast = true>
constexpr auto tanDeg(T degrees) noexcept requires(std::is_integral_v<T>)
{
    return tan<double,fast>(degToRad(double(degrees)));
}


/*
 * Polynomial approximation for ArcTangent(x), returns atan in range [-pi/2,pi/2].
 * It has two modes - fast but lower max accuracy, and vice versa, although this affects
 * only calculations with args lower than switch value, where approximation switches to linear.
 * Fast version has max. absolute error - 0.014, slow version - 0.009, basically
 * former has 1 digit precision in fractional part and latter has 2 digits, but
 * also almost two times faster than slow version in the region before approx. switch.
 */
template <typename T, bool fast = true>
constexpr T atan(T x) noexcept requires(std::is_floating_point_v<T>)
{
    if (x == 0)
        return x;
    const int sign = x >= 0 ? 1 : -1;
    x *= sign;
    if constexpr(fast)// use lower degree polynomial
    {
        if (x > ATAN_APPROX_SWITCH_DEGREE_3)
            return sign * std::min(HALF_PI, ATAN_LINEAR_DEGREE_3_A * x + ATAN_LINEAR_DEGREE_3_B);
        else
        {
            constexpr auto polySize = ATAN_DEGREE_3.size();
            T res = ATAN_DEGREE_3[polySize-1];
            for (int i = polySize-2; i >= 0; --i)
                res = res * x + ATAN_DEGREE_3[i];

            return sign * res;
        }
    }
    else
    {
        if (x > ATAN_APPROX_SWITCH_DEGREE_8)
            return sign * std::min(HALF_PI, ATAN_LINEAR_DEGREE_8_A * x + ATAN_LINEAR_DEGREE_8_B);
        else
        {
            constexpr auto polySize = ATAN_DEGREE_8.size();
            T res = ATAN_DEGREE_8[polySize-1];
            for (int i = polySize-2; i >= 0; --i)
                res = res * x + ATAN_DEGREE_8[i];

            return sign * res;
        }
    }
}

// wrapper function to handle interger arguments
template <typename T, bool fast = true>
constexpr auto atan(T x) noexcept requires(std::is_integral_v<T>)
{
    return atan<double,fast>(x);
}

float sinFast(float x) noexcept
{
    if (x == std::numeric_limits<float>::infinity())
        return std::numeric_limits<float>::signaling_NaN();

    return _OptimizerInternal::sinPtrf(x);
}

double sinFast(double x) noexcept
{
    if (x == std::numeric_limits<double>::infinity())
        return std::numeric_limits<double>::signaling_NaN();

    return _OptimizerInternal::sinPtrd(x);
}

// wrapper function to handle interger arguments
template <typename T>
auto sinFast(T x) noexcept requires(std::is_integral_v<T>)
{
    return sinFast(double(x));
}

float sinFastDeg(float degrees) noexcept
{
    return sinFast(degToRad(degrees));
}

double sinFastDeg(double degrees) noexcept
{
    return sinFast(degToRad(degrees));
}

// wrapper function to handle interger arguments
template <typename T>
constexpr auto sinFastDeg(T degrees) noexcept requires(std::is_integral_v<T>)
{
    return sinFastDeg(degToRad(double(degrees)));
}

float cosFast(float x) noexcept
{
    if (x == std::numeric_limits<float>::infinity())
        return std::numeric_limits<float>::signaling_NaN();

    return _OptimizerInternal::cosPtrf(x);
}

double cosFast(double x) noexcept
{
    if (x == std::numeric_limits<double>::infinity())
        return std::numeric_limits<double>::signaling_NaN();

    return _OptimizerInternal::cosPtrd(x);
}

// wrapper function to handle interger arguments
template <typename T>
auto cosFast(T x) noexcept requires(std::is_integral_v<T>)
{
    return cosFast(double(x));
}

float cosFastDeg(float degrees) noexcept
{
    return cosFast(degToRad(degrees));
}

double cosFastDeg(double degrees) noexcept
{
    return cosFast(degToRad(degrees));
}

// wrapper function to handle interger arguments
template <typename T>
constexpr auto cosFastDeg(T degrees) noexcept requires(std::is_integral_v<T>)
{
    return cosFastDeg(degToRad(double(degrees)));
}

void sinCos(float x, float& sinRes, float& cosRes) noexcept
{
    assert(x != std::numeric_limits<float>::infinity());

    _OptimizerInternal::sinCosPtrf(x, sinRes, cosRes);
}

void sinCos(double x, double& sinRes, double& cosRes) noexcept
{
    assert(x != std::numeric_limits<double>::infinity());

    _OptimizerInternal::sinCosPtrd(x, sinRes, cosRes);
}


}
}
