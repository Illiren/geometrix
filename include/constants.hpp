#pragma once 


// Trigonometry
namespace Geometrix{
inline constexpr auto DEG_TO_RAD = 1.7453292519943295769236907684886E-2;
inline constexpr auto HALF_PI = 1.5707963267948966192;
inline constexpr auto QUARTER_PI = 7.853981633974483096E-1;
inline constexpr auto INV_QUARTER_PI = 1 / QUARTER_PI;
inline constexpr auto INV_HALF_PI = 1 / HALF_PI;
inline constexpr auto TWO_PI = 6.2831853071795864769;

enum Octants
{
    Zero_Pi4     = 0,
    Pi4_Pi2      = 1,
    Pi2_Pi3by4   = 2,
    Pi3by4_Pi    = 3,
    PiZero_Pi4   = 4,
    PiPi4_Pi2    = 5,
    PiPi2_Pi3by4 = 6,
    PiPi3by4_2Pi = 7
};

enum Quads
{
    Zero_Pi2   = 0,
    Pi2_Pi     = 1,
    Pi_Pi3by2  = 2,
    Pi3by2_2Pi = 3
};
}
