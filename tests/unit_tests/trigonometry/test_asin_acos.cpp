/*
 * Accuracy tests for ArcSine and ArcCosine functions
 * for all supported data types (apart from integers, as it wrappes float implementaion)
 */

#include "../../../include/trigonometry.hpp"
#include "../../test_generator.hpp"
#include "../../utility_accuracy.hpp"

// Accuracy test in range of -1..1
inline constexpr auto rangeVal = 1.;
inline constexpr auto stepVal = 0.001;


template<typename T>
void accuracyTest(double expecterError, T(*MeasureFunc)(T), T(*ControlFunc)(T))
{
    std::cout << setprecision(10);
    std::cout << "Accuracy test for maximum error of " << expecterError << std::endl;
    std::vector<T> measure;
    std::vector<T> control;
    for(T i = -rangeVal; i < rangeVal; i+=stepVal)
    {
        measure.push_back(MeasureFunc(i));
        control.push_back(ControlFunc(i));
    }
    // makes sense to check average error because error spikes when approaching
    // range limits -1, 1
    double measuredError = absoluteAverageError(measure, control);
    assert(measuredError <= expecterError);
}


// Test that absolute maximum error is not greater that defined
class ASinAccuracyTester
{
public:
    template <typename T>
    static void test()
    {
        accuracyTest<T>(0.0099, &Geometrix::Trig::asin, &std::asin);
    }
};

class ACosAccuracyTester
{
public:
    template <typename T>
    static void test()
    {
        accuracyTest<T>(0.0099, &Geometrix::Trig::acos, &std::acos);
    }
};


int main()
{
    std::cout << std::endl << "Running ArcSine tests" << std::endl;
    TestGenerator<ASinAccuracyTester, float, double>::test();
    std::cout << std::endl << "Running ArcCosine tests" << std::endl;
    TestGenerator<ACosAccuracyTester, float, double>::test();

    std::cout << std::endl << "ArcSine/ArcCosine tests finished succesfully" << std::endl;
    return 0;
}
