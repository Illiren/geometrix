#include "../../include/matrix.hpp"
#include "../../include/optimizer.hpp"
#include <iostream>


int main()
{
    Geometrix::LA::Matrix<float, 4,4> m1(2);
    Geometrix::LA::Matrix<float, 4,4> m2(1);
    Geometrix::LA::Matrix<float, 4,4> m(0);
    float m3 = 1;
    m = m1 * m2;
    m /= m3;

    Geometrix::Optimizer::init();

    m = m1 * m2;
    m /= m3;

    for (auto it = Geometrix::LA::begin(m); it != Geometrix::LA::end(m); it++)
        std::cout << *it << std::endl;

	std::cout << std::endl << "hello matrix, it has you" << std::endl;
	return 0;
}
