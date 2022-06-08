/*
 * Accuracy tests for Sin and Cos functions for LUT and Poly implementations,
 * for all supported data types (apart from integers, as it wrappes float implementaion
 */

#include "../../../include/trigonometry.hpp"
#include "../../test_generator.hpp"
#include "../../utility_accuracy.hpp"
#include <iostream>

// Accuracy test in 5-period range with 0.01 step
inline constexpr auto rangeVal = 10*M_PI;
inline constexpr auto stepVal = 0.01;


template<typename T>
void accuracyTest(bool polyImpl, std::size_t accuracy, double expecterError, T(*MeasureFunc)(T), T(*ControlFunc)(T))
{
    const char* implStr = polyImpl ? "polynomial impl" : "LUT impl";
    std::cout << setprecision(10);
    std::cout << "Accuracy test (" << implStr << ", num of accurate digits: " << accuracy << ") for maximum error of " << expecterError << std::endl;
    double measuredError;
    std::vector<T> measure;
    std::vector<T> control;
    for(T i = -rangeVal; i < rangeVal; i+=stepVal)
    {
        measure.push_back(MeasureFunc(i));
        control.push_back(ControlFunc(i));
    }
    if (polyImpl)
        measuredError = absoluteMaxError(measure, control);
    else // it makes more sense to test average error, beacuse of discrete nature of LUT
        measuredError = absoluteAverageError(measure, control);
    assert(measuredError <= expecterError);
}


// Test that absolute maximum error is not greater that defined
class SinAccuracyTester
{
public:
    template <typename T>
    static void test()
    {
        accuracyTest<T>(true, 0, 0.1, &Geometrix::Trig::sin<T,0,true>, &std::sin);
        accuracyTest<T>(true, 1, 0.09, &Geometrix::Trig::sin<T,1,true>, &std::sin);
        accuracyTest<T>(true, 2, 0.009, &Geometrix::Trig::sin<T,2,true>, &std::sin);
        accuracyTest<T>(true, 3, 0.0009, &Geometrix::Trig::sin<T,3,true>, &std::sin);
        accuracyTest<T>(true, 4, 0.00009, &Geometrix::Trig::sin<T,4,true>, &std::sin);
        accuracyTest<T>(true, 5, 0.000009, &Geometrix::Trig::sin<T,5,true>, &std::sin);
        if constexpr (std::is_same_v<T, double>)
        { // test higher max accuracy for double
            accuracyTest<T>(true, 6, 0.0000009, &Geometrix::Trig::sin<T,6,true>, &std::sin);
            accuracyTest<T>(true, 7, 0.00000009, &Geometrix::Trig::sin<T,7,true>, &std::sin);
            accuracyTest<T>(true, 8, 0.000000009, &Geometrix::Trig::sin<T,8,true>, &std::sin);
            accuracyTest<T>(true, 9, 0.0000000009, &Geometrix::Trig::sin<T,9,true>, &std::sin);
            accuracyTest<T>(true, 10, 0.0000000009, &Geometrix::Trig::sin<T,10,true>, &std::sin);
        }

        // test table implementation
        accuracyTest<T>(false, 0, 0.99, &Geometrix::Trig::sin<T,0,false>, &std::sin);
        accuracyTest<T>(false, 1, 0.0999, &Geometrix::Trig::sin<T,1,false>, &std::sin);
        accuracyTest<T>(false, 2, 0.00999, &Geometrix::Trig::sin<T,2,false>, &std::sin);
        accuracyTest<T>(false, 3, 0.0009999, &Geometrix::Trig::sin<T,3,false>, &std::sin);
    }
};

class CosAccuracyTester
{
public:
    template <typename T>
    static void test()
    {
        accuracyTest<T>(true, 0, 0.1, &Geometrix::Trig::cos<T,0,true>, &std::cos);
        accuracyTest<T>(true, 1, 0.09, &Geometrix::Trig::cos<T,1,true>, &std::cos);
        accuracyTest<T>(true, 2, 0.009, &Geometrix::Trig::cos<T,2,true>, &std::cos);
        accuracyTest<T>(true, 3, 0.0009, &Geometrix::Trig::cos<T,3,true>, &std::cos);
        accuracyTest<T>(true, 4, 0.00009, &Geometrix::Trig::cos<T,4,true>, &std::cos);
        accuracyTest<T>(true, 5, 0.000009, &Geometrix::Trig::cos<T,5,true>, &std::cos);
        if constexpr (std::is_same_v<T, double>)
        { // test higher max accuracy for double
            accuracyTest<T>(true, 6, 0.0000009, &Geometrix::Trig::cos<T,6,true>, &std::cos);
            accuracyTest<T>(true, 7, 0.00000009, &Geometrix::Trig::cos<T,7,true>, &std::cos);
            accuracyTest<T>(true, 8, 0.000000009, &Geometrix::Trig::cos<T,8,true>, &std::cos);
            accuracyTest<T>(true, 9, 0.0000000009, &Geometrix::Trig::cos<T,9,true>, &std::cos);
            accuracyTest<T>(true, 10, 0.0000000009, &Geometrix::Trig::cos<T,10,true>, &std::cos);
        }

        // test table implementation
        accuracyTest<T>(false, 0, 0.99, &Geometrix::Trig::cos<T,0,false>, &std::cos);
        accuracyTest<T>(false, 1, 0.0999, &Geometrix::Trig::cos<T,1,false>, &std::cos);
        accuracyTest<T>(false, 2, 0.00999, &Geometrix::Trig::cos<T,2,false>, &std::cos);
        accuracyTest<T>(false, 3, 0.0009999, &Geometrix::Trig::cos<T,3,false>, &std::cos);
    }
};


class SinCosAccuracyTester
{
public:
    template <typename T>
    static void test()
    {
        std::vector<T> measureS;
        std::vector<T> controlS;
        std::vector<T> measureC;
        std::vector<T> controlC;
        for(T i = -rangeVal; i < T(rangeVal); i+=T(stepVal))
        {
            T s, c;
            Geometrix::Trig::sinCos(i, s, c);
            measureS.push_back(s);
            measureC.push_back(c);
            s = std::sin(i);
            c = std::cos(i);
            controlS.push_back(s);
            controlC.push_back(c);
        }
        double measuredError = std::max(absoluteMaxError(measureS, controlS), absoluteMaxError(measureC, controlC));
        double expectedError = std::is_same_v<T, float> ? 1E-6 : 1E-14;
        assert(measuredError <= expectedError);
    }
};

int main()
{
    std::cout << std::endl << "Running Sine tests" << std::endl;
    TestGenerator<SinAccuracyTester, float, double>::test();
    std::cout << std::endl << "Running Cosine tests" << std::endl;
    TestGenerator<CosAccuracyTester, float, double>::test();
    std::cout << std::endl << "Running SinCos fallback tests without optimizations" << std::endl;
    TestGenerator<SinCosAccuracyTester, float, double>::test();
    // test with optimizations enabled
    Geometrix::Optimizer::init();
    std::cout << std::endl << "Running Sine tests with optimizations enabled" << std::endl;
    TestGenerator<SinAccuracyTester, float, double>::test();
    std::cout << std::endl << "Running Cosine tests with optimizations enabled" << std::endl;
    TestGenerator<CosAccuracyTester, float, double>::test();
    std::cout << std::endl << "Running SinCos tests with optimizations enabled" << std::endl;
    TestGenerator<SinCosAccuracyTester, float, double>::test();

    std::cout << std::endl << "Sin/Cos tests finished succesfully" << std::endl;
    return 0;
}
