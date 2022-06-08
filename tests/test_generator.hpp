#pragma once
/*
 * File contains utility class test cases generation, involving multiple data types
 *
 * Generator Class has following interface. In order to use it, one has to provide:
 * - a Class, which has test<Type>() static function implemented, containing the test case itself,
 * - one o more type parameters to run the test through, via tail recursion
*/

// class declaration
template <class Tester, class ...Tail> class TestGenerator;

// Specialization for multiple template parameters
template <class Tester, class Head, class ...Tail> class TestGenerator<Tester, Head, Tail...>
{
public:
    constexpr static void test()
    {
        TestGenerator<Tester, Head>::test();
        TestGenerator<Tester, Tail...>::test();
    }
};


// Specialization for single template parameter
template <class Tester, class T>
class TestGenerator<Tester, T>
{
public:
    constexpr static void test()
    {
        Tester::template test<T>();
    }
};
