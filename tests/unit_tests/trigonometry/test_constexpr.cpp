#include "../../../include/trigonometry.hpp"
#include "../../test_generator.hpp"


using namespace Geometrix;
//============================= constexpr tests ==============================//
// Tests whether that constexpr versions are generated for all given functions
// with all supported built-in data types
// It's just has to compile

template <typename T, T (*Func)(T)>
constexpr void constexprLUTTest()
{
    constexpr auto lut1 = Trig::_Internal::generic_inner_table<T, Trig::_Internal::LUTInfo<T, Func, SIN_COS_FOLDING_RATIO,0>>(0);
}

template <typename T>
constexpr void constexprTest()
{
    constexpr auto test1 = Trig::sin<T,0,false>(T(0));
    constexpr auto test2 = Trig::sin<T,0,true>(T(0));
    constexpr auto test3 = Trig::cos<T,0,false>(T(0));
    constexpr auto test4 = Trig::cos<T,0,true>(T(0));
    constexpr auto test5 = Trig::sinDeg<T,0,false>(T(0));
    constexpr auto test6 = Trig::sinDeg<T,0,true>(T(0));
    constexpr auto test7 = Trig::cosDeg<T,0,false>(T(0));
    constexpr auto test8 = Trig::cosDeg<T,0,true>(T(0));
    constexpr auto test9 = Trig::tan<T,true>(T(0));
    constexpr auto test10 = Trig::tan<T,false>(T(0));
    constexpr auto test11 = Trig::atan<T,true>(T(0));
    constexpr auto test12 = Trig::atan<T,false>(T(0));
    constexpr auto test13 = Trig::tanDeg<T,true>(T(0));
    constexpr auto test14 = Trig::tanDeg<T,false>(T(0));
    constexpr auto test15 = Trig::acos<T>(T(0));
    constexpr auto test16 = Trig::asin<T>(T(0));
}


class ConstexprTrigTester
{
public:
    template <typename T>
    constexpr static void test()
    {
        constexprTest<T>();
    }
};


class ConstexprLUTTester
{
public:
    template <typename T>
    constexpr static void test()
    {
        constexprLUTTest<T, &Trig::sin>();
        constexprLUTTest<T, &Trig::cos>();
    }
};


int main()
{
    TestGenerator<ConstexprTrigTester, short, int, long, long long, float, double>::test();
    TestGenerator<ConstexprLUTTester, float, double>::test();
    return 0;
}
