#pragma once
/*
 * File contains utility functions for conducting tests and benchmarks,
 * for measuring accuracy and runtime speed comparisons
 *
*/


#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>

//============================== Accuracy suite ============================//

// enchanced Kahan summation algorithm for negating loss of significance
template<typename T> T preciseSum(const std::vector<T>& input)
{
    static_assert (std::is_floating_point<T>(), "Useful only with floating-point numbers");
    T sum = 0;
    T correction = 0;
    if (input.size() == 0)
        return sum;

    for(const T& i : input)
    {
        T newSum = sum + i;
        if (std::abs(sum) >= std::abs(i))
            correction += (sum - newSum) + i; // compensate lower digits of input
        else
            correction += (i - newSum) + sum; // compensate lower digits of sum
        sum = newSum;
    }

    return sum + correction;
}

template<typename T> T absoluteError(const T& measure, const T& control)
{
    return measure - control;
}

template<typename T> T relativeError(const T& measure, const T& control, bool* exception = nullptr)
{
    if (std::abs(control) == 0)
    {
        if (exception)
            *exception = true;
        return 0;
    }

    return std::abs(1 - std::abs(measure / control));
}

// Root-Mean-Square error
template<typename T> T rmsError(const std::vector<T>& measure, const std::vector<T>& control)
{
    static_assert (std::is_integral<T>() || std::is_floating_point<T>(), "NaN");
    size_t count = measure.size();
    assert(count != 0 && count == control.size() && "invalid arrays sizes!");
    T divided = 0;
    if (std::is_floating_point<T>())
    {// use float optimized sum then
        std::vector<T> sum;
        for(size_t i = 0; i < count; ++i)
            sum.push_back(std::pow(measure[i] - control[i], 2));
        divided = preciseSum(sum);

    }
    else
    {// fallback
        for(size_t i = 0; i < count; ++i)
            divided += (std::pow(measure[i] - control[i], 2));
    }

    return std::sqrt(divided / count);
}

template<typename T> T absoluteMaxError(const std::vector<T>& measure, const std::vector<T>& control)
{
    assert(measure.size() == control.size() && "arrays sizes differ!");
    T maximum = 0;
    for(size_t i = 0; i < measure.size(); ++i)
    {
        const T current = std::abs(measure[i] - control[i]);
        if (current > maximum)
            maximum = current;
    }

    return maximum;
}

template<typename T> T absoluteAverageError(const std::vector<T>& measure, const std::vector<T>& control)
{
    static_assert (std::is_integral<T>() || std::is_floating_point<T>(), "NaN");
    size_t count = measure.size();
    assert(count != 0 && count == control.size() && "invalid arrays sizes!");
    T divided = 0;
    if (std::is_floating_point<T>())
    {// use float optimized sum then
        std::vector<T> sum;
        for(size_t i = 0; i < count; ++i)
            sum.push_back(std::abs(measure[i] - control[i]));
        divided = preciseSum(sum);
    }
    else
    {// fallback
        for(size_t i = 0; i < count; ++i)
            divided += (std::abs(measure[i] - control[i]));
    }

    return divided / count;
}

template<typename T> T relativeMaxError(const std::vector<T>& measure, const std::vector<T>& control)
{
    assert(measure.size() == control.size() && "arrays sizes differ!");
    T maximum = 0;
    for(size_t i = 0; i < measure.size(); ++i)
        if (control[i] != 0)
            maximum = std::max(relativeError(measure[i], control[i]), maximum);

    return maximum;
}

template<typename T> T relativeAverageError(const std::vector<T>& measure, const std::vector<T>& control)
{
    static_assert (std::is_integral<T>() || std::is_floating_point<T>(), "NaN");
    size_t count = measure.size();
    assert(count != 0 && count == control.size() && "invalid arrays sizes!");
    T divided = 0;
    size_t currentCount = count;
    if (std::is_floating_point<T>())
    {// use float optimized sum then
        std::vector<T> sum;
        for(size_t i = 0; i < count; ++i)
        {
            bool except = false;
            const T res = relativeError(measure[i], control[i], &except);
            if (except)
                --currentCount;
            else
                sum.push_back(res);
        }
        divided = preciseSum(sum);
    }
    else
    {// fallback
        for(size_t i = 0; i < count; ++i)
        {
            bool except = false;
            const T res = relativeError(measure[i], control[i], &except);
            if (except)
                --currentCount;
            else
                divided += res;
        }
    }

    return divided / currentCount;
}
