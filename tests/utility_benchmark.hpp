#pragma once
/*
 * File contains utility functions for conducting tests and benchmarks,
 * for measuring accuracy and runtime speed comparisons
 *
*/


#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <functional>
#include <cassert>
#include "utility_accuracy.hpp"


//============================ Benchmarking suite ===========================//
// Time table
template<typename T>
constexpr auto TimeScaleStr = "s";

template<>
constexpr auto TimeScaleStr<std::chrono::nanoseconds> = "nS";
template<>
constexpr auto TimeScaleStr<std::chrono::microseconds> = "mcS";
template<>
constexpr auto TimeScaleStr<std::chrono::milliseconds> = "mS";
template<>
constexpr auto TimeScaleStr<std::chrono::seconds> = "s";
template<>
constexpr auto TimeScaleStr<std::chrono::minutes> = "m";
template<>
constexpr auto TimeScaleStr<std::chrono::hours> = "h";



template<typename T>
void accuracyBench(T start, T range, T step, T(*MeasureFunc)(T), T(*ControlFunc)(T), const char* name)
{
    std::cout << std::endl <<"=========== Accuracy Benchmark " << name << " ===========" << std::endl;
    std::vector<T> measure;
    std::vector<T> control;
    u_int64_t count = 0;
    for(T i = start; i < range; i+=step, ++count)
    {
        measure.push_back(MeasureFunc(i));
        control.push_back(ControlFunc(i));
    }
    std::cout << " number of passes " << std::to_string(count) << std::endl;
    std::cout << "abs error: " << absoluteAverageError(measure, control) << std::endl;
    std::cout << "rel error: " << relativeAverageError(measure, control) << std::endl;
    std::cout << "rms error: " << rmsError(measure, control) << std::endl;
    std::cout << "max abs error: " << absoluteMaxError(measure, control) << std::endl;
    std::cout << "max rel error: " << relativeMaxError(measure, control) << std::endl;
}

template<typename T, typename RandDistr = std::uniform_real_distribution<T>>
void accuracyBenchRand(T min, T max, int size, T(*MeasureFunc)(T), T(*ControlFunc)(T), std::random_device& rnd, const char* name)
{
    assert(min < max && "invalid args");
    std::cout << std::endl <<"=========== Accuracy Benchmark Random " << name << " ============" << std::endl;
    std::vector<T> measure;
    std::vector<T> control;
    measure.resize(size);
    control.resize(size);
    u_int64_t count = 0;
    for(int i = 0; i < size; ++i, ++count)
    {
        RandDistr dist(min, max);
        T arg = dist(rnd);
        measure[i] = (MeasureFunc(arg));
        control[i] = (ControlFunc(arg));
    }
    std::cout << " number of passes " << std::to_string(count) << std::endl;
    std::cout << "abs error: " << absoluteAverageError(measure, control) << std::endl;
    std::cout << "rel error: " << relativeAverageError(measure, control) << std::endl;
    std::cout << "rms error: " << rmsError(measure, control) << std::endl;
    std::cout << "max abs error: " << absoluteMaxError(measure, control) << std::endl;
    std::cout << "max rel error: " << relativeMaxError(measure, control) << std::endl;
}

// returns relative runtime diffence, compared to control func
template<typename T, typename TimeScale = std::chrono::milliseconds>
float speedBench(T start, T range, T step, T(*MeasureFunc)(T), T(*ControlFunc)(T), const char* name)
{
    std::cout << std::endl <<"=========== Speed Benchmark " << name << " ============" << std::endl;
    u_int64_t count = 0;
    for(T i = start; i < range; i+=step, ++count);
    std::cout << " number of passes " << std::to_string(count) << std::endl;

    auto startTime = std::chrono::high_resolution_clock::now();
    for(T i = start; i < range; i+=step)
        MeasureFunc(i);
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<TimeScale>(endTime - startTime);
    std::cout << "Measured duration in " << TimeScaleStr<TimeScale> << ": " <<  duration.count() << std::endl;

    auto startTime2 = std::chrono::high_resolution_clock::now();
    for(T i = start; i < range; i+=step)
        ControlFunc(i);
    auto endTime2 = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<TimeScale>(endTime2 - startTime2);
    std::cout << "Control duration in " << TimeScaleStr<TimeScale> << ": " << duration2.count() << std::endl;
    auto diff = float(duration.count()) / float(duration2.count());
    auto res = (diff > 1.f) ? "slower" : "faster";
    std::cout << ((diff > 1.f) ? diff :  1.f/diff) << " times " << res << " than control function" << std::endl;
    return diff;
}

// returns relative runtime diffence, compared to control func
template<typename T, typename TimeScale = std::chrono::milliseconds, typename RandDistr = std::uniform_real_distribution<T>>
float speedBenchRand(T min, T max, int size, T(*MeasureFunc)(T), T(*ControlFunc)(T), std::random_device& rnd, const char* name)
{
    assert(min < max && "invalid args");
    std::cout << std::endl <<"=========== Speed Benchmark Random " << name << " ============" << std::endl;
    std::vector<T> data;
    data.resize(size);
    u_int64_t count = 0;
    for(int i = 0; i < size; ++i, ++count)
    {
        RandDistr dist(min, max);
        data[i] = dist(rnd);
    }
    std::cout << " number of passes " << std::to_string(count) << std::endl;
    auto startTime = std::chrono::high_resolution_clock::now();
    for(T& i : data)
        MeasureFunc(i);
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<TimeScale>(endTime - startTime);
    std::cout << "Measured duration in " << TimeScaleStr<TimeScale> << ": " << duration.count() << std::endl;
    auto startTime2 = std::chrono::high_resolution_clock::now();
    for(T& i : data)
        ControlFunc(i);
    auto endTime2 = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<TimeScale>(endTime2 - startTime2);
    std::cout << "Control duration in " << TimeScaleStr<TimeScale> << ": " << duration2.count() << std::endl;
    auto diff = float(duration.count()) / float(duration2.count());
    auto res = (diff > 1.f) ? "slower" : "faster";
    std::cout << ((diff > 1.f) ? diff :  1.f/diff) << " times " << res << " than control function" << std::endl;
    return diff;
}
