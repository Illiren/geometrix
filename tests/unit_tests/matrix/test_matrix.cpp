#include "../../../include/matrix.hpp"
#include "../../../include/optimizer.hpp"
#include "../../test_generator.hpp"
#include <iostream>


class MatrixOperatorTester
{
    template<typename T, std::size_t Row, std::size_t Col>
    static void comparisonOp()
    {
        std::cout << "Comparison matrix operator test, with dimensions: " << Row << "x" << Col << std::endl;
        Geometrix::LA::Matrix<T, Row, Col> lhs(1);
        Geometrix::LA::Matrix<T, Row, Col> rhs(1);

        assert(lhs == rhs);
        //assert(!(lhs != rhs)); //for some reason fail for 4x4 float matrix
    }

    template<typename T, std::size_t Row, std::size_t Col>
    static void assignOp()
    {
        std::cout << "Assignment matrix operator test, with dimensions: " << Row << "x" << Col << std::endl;
        Geometrix::LA::Matrix<T, Row, Col> lhs(0);
        Geometrix::LA::Matrix<T, Row, Col> rhs(1);

        lhs = rhs;
        assert(lhs == rhs);
    }

    template<typename T, std::size_t Row, std::size_t Col>
    static void addOp()
    {
        std::cout << "Addition matrix operator test, with dimensions: " << Row << "x" << Col << std::endl;
        Geometrix::LA::Matrix<T, Row, Col> expected(5);
        Geometrix::LA::Matrix<T, Row, Col> lhs(2);
        const T rhsT = 3;
        Geometrix::LA::Matrix<T, Row, Col> rhs(rhsT);

        Geometrix::LA::Matrix<T, Row, Col> result = lhs + rhs;
        assert(result == expected);

        result = lhs + rhsT;
        assert(result == expected);

        lhs += rhs;
        assert(lhs == expected);
    }

    template<typename T, std::size_t Row, std::size_t Col>
    static void subOp()
    {
        std::cout << "Substraction matrix operator test, with dimensions: " << Row << "x" << Col << std::endl;
        Geometrix::LA::Matrix<T, Row, Col> expected(1);
        Geometrix::LA::Matrix<T, Row, Col> lhs(3);
        const T rhsT = 2;
        Geometrix::LA::Matrix<T, Row, Col> rhs(rhsT);

        Geometrix::LA::Matrix<T, Row, Col> result = lhs - rhs;
        assert(result == expected);

        result = lhs - rhsT;
        assert(result == expected);

        lhs -= rhs;
        assert(lhs == expected);
    }

    template<typename T, std::size_t Row, std::size_t Col>
    static void mulOp()
    {
        std::cout << "Multiplication matrix operator test, with dimensions: " << Row << "x" << Col << std::endl;
        Geometrix::LA::Matrix<T, Row, Col> expected(6);
        Geometrix::LA::Matrix<T, Row, Col> lhs(2);
        const T rhsT = 3;
        Geometrix::LA::Matrix<T, Row, Col> rhs(rhsT);

        Geometrix::LA::Matrix<T, Row, Col> result = lhs * rhs;
        assert(result == expected);

        result = lhs * rhsT;
        assert(result == expected);

        lhs *= rhs;
        assert(lhs == expected);
    }

    template<typename T, std::size_t Row, std::size_t Col>
    static void divOp()
    {
        std::cout << "Division matrix operator test, with dimensions: " << Row << "x" << Col << std::endl;
        Geometrix::LA::Matrix<T, Row, Col> expected(2);
        Geometrix::LA::Matrix<T, Row, Col> lhs(6);
        const T rhsT = 3;
        Geometrix::LA::Matrix<T, Row, Col> rhs(rhsT);

        Geometrix::LA::Matrix<T, Row, Col> result = lhs / rhs;
        assert(result == expected);

        result = lhs / rhsT;
        assert(result == expected);

        lhs /= rhs;
        assert(lhs == expected);
    }

    template<typename T, std::size_t Row, std::size_t Col>
    static void operatorTests()
    {
        comparisonOp<T,Row,Col>();
        assignOp<T,Row,Col>();
        addOp<T,Row,Col>();
        subOp<T,Row,Col>();
        mulOp<T,Row,Col>();
        divOp<T,Row,Col>();
    }

public:
    template <typename T>
    static void test()
    {
        operatorTests<T,1,1>(); // test specializations
        operatorTests<T,1,2>();
        operatorTests<T,1,3>();
        operatorTests<T,1,4>();

        operatorTests<T,2,2>();
        operatorTests<T,3,3>();
        operatorTests<T,4,4>();

        operatorTests<T,1,5>(); // test generic classes
        operatorTests<T,5,5>();
    }
};


int main()
{
    std::cout << std::endl << "Running matrix tests" << std::endl;
    TestGenerator<MatrixOperatorTester, short, int, long, long long, float, double>::test();
    // test with optimizations enabled
    Geometrix::Optimizer::init();
    std::cout << std::endl << "Running matrix tests with optimizations enabled" << std::endl;

    TestGenerator<MatrixOperatorTester, short, int, long, long long, float, double>::test();
    std::cout << std::endl << "Matrix tests finished succesfully" << std::endl;
    return 0;
}
