/*
 * Accuracy tests for Tangent and ArcTanget functions and it's implementations
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
void accuracyTest(bool fastVersion, double expecterError, T(*MeasureFunc)(T), T(*ControlFunc)(T))
{
    std::cout << setprecision(10);
    std::cout << "Accuracy test (" << (fastVersion ? "optimized" : "not optimized") << ") for maximum error of " << expecterError << std::endl;
    std::vector<T> measure;
    std::vector<T> control;
    for(T i = -rangeVal; i < rangeVal; i+=stepVal)
    {
        measure.push_back(MeasureFunc(i));
        control.push_back(ControlFunc(i));
    }
    // makes sense to check average error because aTan has spikes in error values
    // when function approaches infinity
    // and use relative error as output values are really small
    double measuredError = relativeAverageError(measure, control);
    assert(measuredError <= expecterError);
}


// Test that absolute maximum error is not greater that defined
class TanAccuracyTester
{
public:
    template <typename T>
    static void test()
    {
        // test faster implementation
        accuracyTest<T>(true, 0.0099, &Geometrix::Trig::tan<T,true>, &std::tan);
        // test slower implementation
        accuracyTest<T>(false, 0.0099, &Geometrix::Trig::tan<T,false>, &std::tan);
    }
};

class AtanAccuracyTester
{
public:
    template <typename T>
    static void test()
    {
        // test faster implementation
        accuracyTest<T>(true, 0.099, &Geometrix::Trig::atan<T,true>, &std::atan);
        // test slower implementation
        accuracyTest<T>(false, 0.0099, &Geometrix::Trig::atan<T,false>, &std::atan);
    }
};


int main()
{
    std::cout << std::endl << "Running Tangent tests" << std::endl;
    TestGenerator<TanAccuracyTester, float, double>::test();
    std::cout << std::endl << "Running ArcTanget tests" << std::endl;
    TestGenerator<AtanAccuracyTester, float, double>::test();

    std::cout << std::endl << "Tangent/ArcTanget tests finished succesfully" << std::endl;
    return 0;
}
