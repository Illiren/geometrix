#include "../utility_benchmark.hpp"
#include "../../include/trigonometry.hpp"
#include "../../include/optimizer.hpp"


using namespace Geometrix;
//====================== accuracy and speed tests ============================//

inline constexpr auto runCount = 1000000;
inline constexpr auto rangeVal = 5000*M_PI;
inline constexpr auto stepVal = 0.01;
inline constexpr auto periodRange = 2*M_PI;

template <typename T>
void accuracyValuesSin()
{
    const char* type;
    if constexpr(std::is_same_v<T,float>)
        type = "float";
    else
        type = "double";
    std::cout << std::endl <<"============== 0, Pi/4, Pi/2,...2*Pi Accuracy range test " << type << " ==============" << std::endl;
    T start = 0;
    T step = M_PI_4;
    accuracyBench(start,T(periodRange),step,&Trig::sin<T, sinCosAcc<T>, false>, &std::sin,"table implementation");
    accuracyBench(start,T(periodRange),step,&Trig::sin<T>, &std::sin,"polynomial implementation");
}

template <typename T>
void accuracyValuesCos()
{
    const char* type;
    if constexpr(std::is_same_v<T,float>)
        type = "float";
    else
        type = "double";
    std::cout << std::endl <<"============== 0, Pi/4, Pi/2,...2*Pi Accuracy range test " << type << " ==============" << std::endl;
    T start = 0;
    T step = M_PI_4;
    accuracyBench(start,T(periodRange),step,&Trig::cos<T, sinCosAcc<T>, false>, &std::cos,"table implementation");
    accuracyBench(start,T(periodRange),step,&Trig::cos<T>, &std::cos,"polynomial implementation");
}

void accuracyRangeTestsSin(std::random_device& r)
{
    accuracyBench(float(-rangeVal),float(rangeVal),float(stepVal), &Trig::sin<float, sinCosAcc<float>, false>, &std::sin,"float table implementation");
    accuracyBench(-rangeVal,rangeVal,stepVal,&Trig::sin<double, sinCosAcc<double>, false>, &std::sin,"double table implementation");
    accuracyBench(float(-rangeVal),float(rangeVal),float(stepVal),&Trig::sin<float>, &std::sin,"float test polynomial implementation");
    accuracyBench(-rangeVal,rangeVal,stepVal,&Trig::sin<double>, &std::sin,"double test polynomial implementation");

    accuracyBenchRand(float(-rangeVal),float(rangeVal),runCount, &Trig::sin<float, sinCosAcc<float>, false>, &std::sin,r,"float RAND table implementation");
    accuracyBenchRand(-rangeVal,rangeVal,runCount,&Trig::sin<double, sinCosAcc<double>, false>, &std::sin,r,"double RAND table implementation");
    accuracyBenchRand(float(-rangeVal),float(rangeVal),runCount,&Trig::sin<float>, &std::sin,r,"float test RAND polynomial implementation");
    accuracyBenchRand(-rangeVal,rangeVal,runCount,&Trig::sin<double>, &std::sin,r,"double test RAND polynomial implementation");
}

void speedTestsSin(std::random_device& r)
{
    speedBench(float(-rangeVal),float(rangeVal),float(stepVal), &Trig::sin<float, sinCosAcc<float>, false>, &std::sin,"float test table implementation");
    speedBench(-rangeVal,rangeVal,stepVal,&Trig::sin<double, sinCosAcc<double>, false>, &std::sin,"double test table implementation");
    speedBench(float(-rangeVal),float(rangeVal),float(stepVal),&Trig::sin<float>, &std::sin,"float test polynomial implementation");
    speedBench(-rangeVal,rangeVal,stepVal,&Trig::sin<double>, &std::sin,"double test polynomial implementation");

    /* unreliable
    speedBenchRand(float(-rangeVal),float(rangeVal),runCount, &Trig::sin<float, false>, &std::sin,r,"float table implementation");
    speedBenchRand(-rangeVal,rangeVal,runCount, &Trig::sin<double, false>, &std::sin,r,"double table implementation");
    speedBenchRand(float(-rangeVal),float(rangeVal),runCount, &Trig::sin<float, true>, &std::sin,r,"float polynomial implementation");
    speedBenchRand(-rangeVal,rangeVal,runCount, &Trig::sin<double, true>, &std::sin,r,"double polynomial implementation");
    */
}

void accuracyRangeTestsCos(std::random_device& r)
{
    accuracyBench(float(-rangeVal),float(rangeVal),float(stepVal), &Trig::sin<float, sinCosAcc<float>, false>, &std::sin,"float table implementation");
    accuracyBench(-rangeVal,rangeVal,stepVal,&Trig::sin<double, sinCosAcc<double>, false>, &std::sin,"double table implementation");
    accuracyBench(float(-rangeVal),float(rangeVal),float(stepVal),&Trig::sin<float>, &std::sin,"float test polynomial implementation");
    accuracyBench(-rangeVal,rangeVal,stepVal,&Trig::sin<double>, &std::sin,"double test polynomial implementation");

    accuracyBenchRand(float(-rangeVal),float(rangeVal),runCount, &Trig::sin<float, sinCosAcc<float>, false>, &std::sin,r,"float RAND table implementation");
    accuracyBenchRand(-rangeVal,rangeVal,runCount,&Trig::sin<double, sinCosAcc<double>, false>, &std::sin,r,"double RAND table implementation");
    accuracyBenchRand(float(-rangeVal),float(rangeVal),runCount,&Trig::sin<float>, &std::sin,r,"float test RAND polynomial implementation");
    accuracyBenchRand(-rangeVal,rangeVal,runCount,&Trig::sin<double>, &std::sin,r,"double test RAND polynomial implementation");
}

void speedTestsCos(std::random_device& r)
{
    speedBench(float(-rangeVal),float(rangeVal),float(stepVal), &Trig::sin<float, sinCosAcc<float>, false>, &std::sin,"float test table implementation");
    speedBench(-rangeVal,rangeVal,stepVal,&Trig::sin<double, sinCosAcc<double>, false>, &std::sin,"double test table implementation");
    speedBench(float(-rangeVal),float(rangeVal),float(stepVal),&Trig::sin<float>, &std::sin,"float test polynomial implementation");
    speedBench(-rangeVal,rangeVal,stepVal,&Trig::sin<double>, &std::sin,"double test polynomial implementation");
    /* unreliable
    speedBenchRand(float(-rangeVal),float(rangeVal),runCount, &Trig::sin<float, false>, &std::sin,r,"float table implementation");
    speedBenchRand(-rangeVal,rangeVal,runCount, &Trig::sin<double, false>, &std::sin,r,"double table implementation");
    speedBenchRand(float(-rangeVal),float(rangeVal),runCount, &Trig::sin<float, true>, &std::sin,r,"float polynomial implementation");
    speedBenchRand(-rangeVal,rangeVal,runCount, &Trig::sin<double, true>, &std::sin,r,"double polynomial implementation");
    */
}

void sinCosPolyAccuracyTests(bool sin)
{
    std::cout << std::endl <<"============== Polynomials Accuracy test by number of terms" << " ==============" << std::endl;
    double start = 0;
    double step = stepVal;
    // have to do it the old way
    if (sin)
    {
        accuracyBench(start,periodRange,step,&Trig::sin<double,0>, &std::sin, std::string("sin, number of terms: " +
                                                                                                        std::to_string(std::get<0>(SIN_POLIES[1]))).c_str());
        accuracyBench(start,periodRange,step,&Trig::sin<double,1>, &std::sin, std::string("sin, number of terms: " +
                                                                                                        std::to_string(std::get<0>(SIN_POLIES[2]))).c_str());
        accuracyBench(start,periodRange,step,&Trig::sin<double,2>, &std::sin, std::string("sin, number of terms: " +
                                                                                                        std::to_string(std::get<0>(SIN_POLIES[3]))).c_str());
        accuracyBench(start,periodRange,step,&Trig::sin<double,4>, &std::sin, std::string("sin, number of terms: " +
                                                                                                        std::to_string(std::get<0>(SIN_POLIES[4]))).c_str());
        accuracyBench(start,periodRange,step,&Trig::sin<double,6>, &std::sin, std::string("sin, number of terms: " +
                                                                                                        std::to_string(std::get<0>(SIN_POLIES[5]))).c_str());
        accuracyBench(start,periodRange,step,&Trig::sin<double,7>, &std::sin, std::string("sin, number of terms: " +
                                                                                                        std::to_string(std::get<0>(SIN_POLIES[6]))).c_str());
        accuracyBench(start,periodRange,step,&Trig::sin<double,9>, &std::sin, std::string("sin, number of terms: " +
                                                                                                        std::to_string(std::get<0>(SIN_POLIES[7]))).c_str());
    }
    else
    {
        accuracyBench(start,periodRange,step,&Trig::cos<double,0>, &std::cos,std::string("cos, number of terms: " +
                                                                                                       std::to_string(std::get<0>(COS_POLIES[1]))).c_str());
        accuracyBench(start,periodRange,step,&Trig::cos<double,1>, &std::cos,std::string("cos, number of terms: " +
                                                                                                       std::to_string(std::get<0>(COS_POLIES[2]))).c_str());
        accuracyBench(start,periodRange,step,&Trig::cos<double,2>, &std::cos,std::string("cos, number of terms: " +
                                                                                                       std::to_string(std::get<0>(COS_POLIES[3]))).c_str());
        accuracyBench(start,periodRange,step,&Trig::cos<double,4>, &std::cos,std::string("cos, number of terms: " +
                                                                                                       std::to_string(std::get<0>(COS_POLIES[4]))).c_str());
        accuracyBench(start,periodRange,step,&Trig::cos<double,6>, &std::cos,std::string("cos, number of terms: " +
                                                                                                      std::to_string(std::get<0>(COS_POLIES[5]))).c_str());
        accuracyBench(start,periodRange,step,&Trig::cos<double,7>, &std::cos,std::string("cos, number of terms: " +
                                                                                                       std::to_string(std::get<0>(COS_POLIES[6]))).c_str());
        accuracyBench(start,periodRange,step,&Trig::cos<double,9>, &std::cos,std::string("cos, number of terms: " +
                                                                                                       std::to_string(std::get<0>(COS_POLIES[7]))).c_str());
    }
}

void sinCosPolyPerfTests(bool sin)
{
    std::cout << std::endl <<"============== Polynomials Speed test by number of terms" << " ==============" << std::endl;
    // have to do it the old way
    if (sin)
    {
        speedBench(-rangeVal,rangeVal,stepVal,&Trig::sin<double,0>, &std::sin, std::string("sin, number of terms: " +
                                                                                                        std::to_string(std::get<0>(SIN_POLIES[1]))).c_str());
        speedBench(-rangeVal,rangeVal,stepVal,&Trig::sin<double,1>, &std::sin, std::string("sin, number of terms: " +
                                                                                                        std::to_string(std::get<0>(SIN_POLIES[2]))).c_str());
        speedBench(-rangeVal,rangeVal,stepVal,&Trig::sin<double,2>, &std::sin, std::string("sin, number of terms: " +
                                                                                                        std::to_string(std::get<0>(SIN_POLIES[3]))).c_str());
        speedBench(-rangeVal,rangeVal,stepVal,&Trig::sin<double,4>, &std::sin, std::string("sin, number of terms: " +
                                                                                                        std::to_string(std::get<0>(SIN_POLIES[4]))).c_str());
        speedBench(-rangeVal,rangeVal,stepVal,&Trig::sin<double,6>, &std::sin, std::string("sin, number of terms: " +
                                                                                                        std::to_string(std::get<0>(SIN_POLIES[5]))).c_str());
        speedBench(-rangeVal,rangeVal,stepVal,&Trig::sin<double,7>, &std::sin, std::string("sin, number of terms: " +
                                                                                                        std::to_string(std::get<0>(SIN_POLIES[6]))).c_str());
        speedBench(-rangeVal,rangeVal,stepVal,&Trig::sin<double,9>, &std::sin, std::string("sin, number of terms: " +
                                                                                                        std::to_string(std::get<0>(SIN_POLIES[7]))).c_str());
    }
    else
    {
        speedBench(-rangeVal,rangeVal,stepVal,&Trig::cos<double,0>, &std::cos,std::string("cos, number of terms: " +
                                                                                                       std::to_string(std::get<0>(COS_POLIES[1]))).c_str());
        speedBench(-rangeVal,rangeVal,stepVal,&Trig::cos<double,1>, &std::cos,std::string("cos, number of terms: " +
                                                                                                       std::to_string(std::get<0>(COS_POLIES[2]))).c_str());
        speedBench(-rangeVal,rangeVal,stepVal,&Trig::cos<double,2>, &std::cos,std::string("cos, number of terms: " +
                                                                                                       std::to_string(std::get<0>(COS_POLIES[3]))).c_str());
        speedBench(-rangeVal,rangeVal,stepVal,&Trig::cos<double,4>, &std::cos,std::string("cos, number of terms: " +
                                                                                                       std::to_string(std::get<0>(COS_POLIES[4]))).c_str());
        speedBench(-rangeVal,rangeVal,stepVal,&Trig::cos<double,6>, &std::cos,std::string("cos, number of terms: " +
                                                                                                      std::to_string(std::get<0>(COS_POLIES[5]))).c_str());
        speedBench(-rangeVal,rangeVal,stepVal,&Trig::cos<double,7>, &std::cos,std::string("cos, number of terms: " +
                                                                                                       std::to_string(std::get<0>(COS_POLIES[6]))).c_str());
        speedBench(-rangeVal,rangeVal,stepVal,&Trig::cos<double,9>, &std::cos,std::string("cos, number of terms: " +
                                                                                                       std::to_string(std::get<0>(COS_POLIES[7]))).c_str());
    }
}

std::string tableSizeStr(std::size_t accuracy)
{
    std::size_t size = Trig::_Internal::constLUTSizeFromAcc(Trig::_Internal::SC_LUT_ACC_MAP[accuracy],SIN_COS_FOLDING_RATIO);
    return std::to_string(size);
}
void sinCosTableAccuracyTests(bool isSin)
{
    std::cout << std::endl <<"============== Table Implementation Accuracy test by it's size" << " ==============" << std::endl;
    double start = 0;
    double step = stepVal;
    if (isSin)
    {
        accuracyBench(start,periodRange,step,&Trig::sin<double, 0, false>, &std::sin,std::string("double table implementation; digits of accuracy: 0; size: " + tableSizeStr(0)).c_str());
        accuracyBench(start,periodRange,step,&Trig::sin<double, 1, false>, &std::sin,std::string("double table implementation; digits of accuracy: 1; size: " + tableSizeStr(1)).c_str());
        accuracyBench(start,periodRange,step,&Trig::sin<double, 2, false>, &std::sin,std::string("double table implementation; digits of accuracy: 2; size: " + tableSizeStr(2)).c_str());
        accuracyBench(start,periodRange,step,&Trig::sin<double, 3, false>, &std::sin,std::string("double table implementation; digits of accuracy: 3; size: " + tableSizeStr(3)).c_str());
        accuracyBench(start,periodRange,step,&Trig::sin<double, 4, false>, &std::sin,std::string("double table implementation; digits of accuracy: 4; size: " + tableSizeStr(4)).c_str());
    }
    else
    {
        accuracyBench(start,periodRange,step,&Trig::cos<double, 0, false>, &std::cos,std::string("double table implementation; digits of accuracy: 0; size: " + tableSizeStr(0)).c_str());
        accuracyBench(start,periodRange,step,&Trig::cos<double, 1, false>, &std::cos,std::string("double table implementation; digits of accuracy: 1; size: " + tableSizeStr(1)).c_str());
        accuracyBench(start,periodRange,step,&Trig::cos<double, 2, false>, &std::cos,std::string("double table implementation; digits of accuracy: 2; size: " + tableSizeStr(2)).c_str());
        accuracyBench(start,periodRange,step,&Trig::cos<double, 3, false>, &std::cos,std::string("double table implementation; digits of accuracy: 3; size: " + tableSizeStr(3)).c_str());
        accuracyBench(start,periodRange,step,&Trig::cos<double, 4, false>, &std::cos,std::string("double table implementation; digits of accuracy: 4; size: " + tableSizeStr(4)).c_str());
    }
}

void tanTests(std::random_device& r)
{
    accuracyBench(-rangeVal,rangeVal,stepVal,&Trig::tan<double,true>, &std::tan,"tan, fast version");
    accuracyBench(-rangeVal,rangeVal,stepVal,&Trig::tan<double,false>, &std::tan,"tan, slow version");
    accuracyBenchRand(-rangeVal,rangeVal,runCount,&Trig::tan<double, true>, &std::tan,r,"RAND tan, fast version");
    accuracyBenchRand(-rangeVal,rangeVal,runCount,&Trig::tan<double, false>, &std::tan,r,"RAND tan, slow version");
    speedBench<double,std::chrono::microseconds>(-rangeVal,rangeVal,stepVal,&Trig::tan<double,true>, &std::tan,"tan, fast version");
    speedBench<double,std::chrono::microseconds>(-rangeVal,rangeVal,stepVal,&Trig::tan<double,false>, &std::tan,"tan, slow version");
}

void atanTests(std::random_device& r)
{
    accuracyBench(-rangeVal,rangeVal,stepVal,&Trig::atan<double,true>, &std::atan,"atan, fast version");
    accuracyBench(-rangeVal,rangeVal,stepVal,&Trig::atan<double,false>, &std::atan,"atan, slow version");
    accuracyBenchRand(-rangeVal,rangeVal,runCount,&Trig::atan<double, true>, &std::atan,r,"RAND atan, fast version");
    accuracyBenchRand(-rangeVal,rangeVal,runCount,&Trig::atan<double, false>, &std::atan,r,"RAND atan, slow version");
    std::cout << std::endl <<"============== Speed test inside higher degree polynomial arg range" << " ==============" << std::endl;
    speedBench<double,std::chrono::microseconds>(-double(ATAN_APPROX_SWITCH_DEGREE_3),ATAN_APPROX_SWITCH_DEGREE_3,0.0001,&Trig::atan<double,true>, &Trig::atan,"atan, fast version");
    speedBench<double,std::chrono::microseconds>(-double(ATAN_APPROX_SWITCH_DEGREE_8),ATAN_APPROX_SWITCH_DEGREE_8,0.0001,&Trig::atan<double,false>, &Trig::atan,"atan, slow version");
    std::cout << std::endl <<"============== Speed test for general arg range" << " ==============" << std::endl;
    speedBench<double,std::chrono::microseconds>(-rangeVal, rangeVal, stepVal,&Trig::atan<double,true>, &Trig::atan,"atan, fast version");
    speedBench<double,std::chrono::microseconds>(-rangeVal, rangeVal, stepVal,&Trig::atan<double,false>, &Trig::atan,"atan, slow version");
}

void asinTests(std::random_device& r)
{
    accuracyBench(-1.,1.,0.0001,&Trig::asin, &std::asin,"asin");
    accuracyBenchRand(-1.,1.,runCount,&Trig::asin, &std::asin,r,"RAND asin");
    speedBench<double,std::chrono::nanoseconds>(-1.,1.,0.0001,&Trig::asin, &Trig::asin,"asin");
}

void acosTests(std::random_device& r)
{
    accuracyBench(-1.,1.,0.0001,&Trig::acos, &std::acos,"acos");
    accuracyBenchRand(-1.,1.,runCount,&Trig::acos, &std::acos,r,"RAND acos");
    speedBench<double,std::chrono::nanoseconds>(-1.,1.,0.0001,&Trig::acos, &Trig::acos,"acos");
}

#if defined(__SSE2__) && defined(__FMA__)
void intrinFuncTests()
{
    accuracyBench(float(-rangeVal), float(rangeVal), float(stepVal), &Trig::sinFast, &std::sin, "float sin");
    accuracyBench(-rangeVal, rangeVal, stepVal, &Trig::sinFast, &std::sin, "double sin");
    speedBench(float(-rangeVal),float(rangeVal),float(stepVal), &Trig::sinFast, &std::sin,"float sin");
    speedBench(-rangeVal,rangeVal,stepVal, &Trig::sinFast, &std::sin,"double sin");

    accuracyBench(float(-rangeVal), float(rangeVal), float(stepVal), &Trig::cosFast, &std::cos, "float cos");
    accuracyBench(-rangeVal, rangeVal, stepVal, &Trig::cosFast, &std::cos, "double cos");
    speedBench(float(-rangeVal),float(rangeVal),float(stepVal), &Trig::cosFast, &std::cos,"float cos");
    speedBench(-rangeVal,rangeVal,stepVal, &Trig::cosFast, &std::cos,"double cos");
}

template <typename T>
void sinCosAccBench()
{
    std::vector<T> measureS;
    std::vector<T> controlS;
    std::vector<T> measureC;
    std::vector<T> controlC;
    u_int64_t count = 0;
    for(T i = -rangeVal; i < T(rangeVal); i+=T(stepVal), ++count)
    {
        T s, c;
        Trig::sinCos(i, s, c);
        measureS.push_back(s);
        measureC.push_back(c);
        s = std::sin(i);
        c = std::cos(i);
        controlS.push_back(s);
        controlC.push_back(c);
    }
    std::cout << " number of passes " << std::to_string(count) << std::endl;
    std::cout << "abs error: " << std::max(absoluteAverageError(measureS, controlS), absoluteAverageError(measureC, controlC)) << std::endl;
    std::cout << "rel error: " << std::max(relativeAverageError(measureS, controlS), relativeAverageError(measureC, controlC)) << std::endl;
    std::cout << "rms error: " << std::max(rmsError(measureS, controlS), rmsError(measureC, controlC)) << std::endl;
    std::cout << "max abs error: " << std::max(absoluteMaxError(measureS, controlS), absoluteMaxError(measureC, controlC)) << std::endl;
    std::cout << "max rel error: " << std::max(relativeMaxError(measureS, controlS), relativeMaxError(measureC, controlC)) << std::endl;
}

template <typename T>
void sinCosSpeedBench()
{
    T start = -rangeVal;
    T range = rangeVal;
    T step = stepVal;
    u_int64_t count = 0;
    for(T i = start; i < range; i+=step, ++count);
    std::cout << " number of passes " << std::to_string(count) << std::endl;
    T s, c;
    auto startTime = std::chrono::high_resolution_clock::now();
    for(T i = start; i < range; i+=step)
        Trig::sinCos(i, s, c);
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    std::cout << "Measured duration in " << TimeScaleStr<std::chrono::milliseconds> << ": " <<  duration.count() << std::endl;

    auto startTime2 = std::chrono::high_resolution_clock::now();
    for(T i = start; i < range; i+=step)
    {
        s = std::sin(i);
        c = std::cos(i);
    }
    auto endTime2 = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(endTime2 - startTime2);
    std::cout << "Control duration in " << TimeScaleStr<std::chrono::milliseconds> << ": " << duration2.count() << std::endl;
    auto diff = float(duration.count()) / float(duration2.count());
    auto res = (diff > 1.f) ? "slower" : "faster";
    std::cout << ((diff > 1.f) ? diff : 1.f/diff) << " times " << res << " than control function" << std::endl;
}

void sinCosTests()
{
    std::cout << std::endl <<"=========== Accuracy Benchmark for sinCos float version ============" << std::endl;
    sinCosAccBench<float>();
    std::cout << std::endl <<"=========== Speed Benchmark for sinCos float version ============" << std::endl;
    sinCosSpeedBench<float>();
    std::cout << std::endl <<"=========== Accuracy Benchmark for sinCos double version ============" << std::endl;
    sinCosAccBench<double>();
    std::cout << std::endl <<"=========== Speed Benchmark for sinCos double version ============" << std::endl;
    sinCosSpeedBench<double>();
}
#endif


int main()
{    
    Optimizer::init();
    const char* sep = "========================================================================";
    const char* sepBrackets = "========================";
    std::random_device r;

    std::cout << std::endl << sep << std::endl << sepBrackets << " Sine Benchmark " << sepBrackets << std::endl;
    accuracyRangeTestsSin(r);
    accuracyValuesSin<float>();
    accuracyValuesSin<double>();
    speedTestsSin(r);
    sinCosPolyAccuracyTests(true);
    sinCosPolyPerfTests(true);
    sinCosTableAccuracyTests(true);
    std::cout << std::endl << sep << std::endl << sepBrackets << " END Sine Benchmark " << sepBrackets << std::endl;

    std::cout << std::endl << sep << std::endl << sepBrackets << " Cosine Benchmark " << sepBrackets << std::endl;
    accuracyRangeTestsCos(r);
    accuracyValuesCos<float>();
    accuracyValuesCos<double>();
    speedTestsCos(r);
    sinCosPolyAccuracyTests(false);
    sinCosPolyPerfTests(false);
    sinCosTableAccuracyTests(false);
    std::cout << std::endl << sep << std::endl << sepBrackets << " END Cosine Benchmark " << sepBrackets << std::endl;

    std::cout << std::endl << sep << std::endl << sepBrackets << " Tangent Benchmark " << sepBrackets << std::endl;
    tanTests(r);
    std::cout << std::endl << sep << std::endl << sepBrackets << " END Tangent Benchmark " << sepBrackets << std::endl;

    std::cout << std::endl << sep << std::endl << sepBrackets << " Arc Tangent Benchmark " << sepBrackets << std::endl;
    atanTests(r);
    std::cout << std::endl << sep << std::endl << sepBrackets << " END Arc Tangent Benchmark " << sepBrackets << std::endl;

    std::cout << std::endl << sep << std::endl << sepBrackets << " Arc Sine Benchmark " << sepBrackets << std::endl;
    asinTests(r);
    std::cout << std::endl << sep << std::endl << sepBrackets << " END Arc SineBenchmark " << sepBrackets << std::endl;

    std::cout << std::endl << sep << std::endl << sepBrackets << " Arc Cosine Benchmark " << sepBrackets << std::endl;
    acosTests(r);
    std::cout << std::endl << sep << std::endl << sepBrackets << " END Arc Cosine Benchmark " << sepBrackets << std::endl;
#if defined(__SSE2__) && defined(__FMA__)
    std::cout << std::endl << sep << std::endl << sepBrackets << " SSE versions of sin and cos Benchmark " << sepBrackets << std::endl;
    intrinFuncTests();
    std::cout << std::endl << sep << std::endl << sepBrackets << " END SSE verions of sin and cos Benchmark " << sepBrackets << std::endl;

    std::cout << std::endl << sep << std::endl << sepBrackets << " sinCos Benchmark " << sepBrackets << std::endl;
    sinCosTests();
    std::cout << std::endl << sep << std::endl << sepBrackets << " END sinCos Benchmark " << sepBrackets << std::endl;
#endif
    return 0;
}
