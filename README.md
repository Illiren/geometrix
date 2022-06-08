# Geometrix
A header-only C++ mathematical libary, containing geometry, linear algebra and 
trigonometry functions. With no dependencies.
## Features
  * optimized operations with geometric primitives (which leverage built-in linear algebra 
  and trigonometry)
  * optimized all-purpose vector and matrix representations
  * x86_64 intrinsics implementations of functions
  * run-time detection and enabling of optimized versions of algorithms
  * constexpr-time evaluated trigonometry functions
  * customizable constexpr lookup table generator
  * testing and benchmarking utilities
  * test cases for accuracy and speed benchmarks for trigonometry functions
## Description
### Geometry Utilities
Intersecton alrgorithms for Vector, Circle, Rectangle.
### Linear Algebra Utilities
There is a generic Matrix implementation and several specializations, like Vectors in 
different dimentions, and it's uiilities, like determinant, inversion, dot/cross-produts.

Optimizer notes  
There are matrix operators optimizations, using SSE registers.
### Trigonometry Utilities
For sine and cosine there are two implementations to compare against each other: 
lookup-table-based and polynom-based.  
 * LUT is generated at compile-time, given desired accuracy (which derives it's size). 
Table sizes where chosen based on desired accuracy (function was folded over 1/8 of the 
period, so as tables). Additionally gradients between data points 
were calculated for linear approximation to boost accuracy.

 * The other implementation uses MiniMax Polynomial Approximations generated for different 
degrees of accuracy. The more accurate the longer it takes to compute. There is an option 
to set the desired accuracy at compile-time.

Tangent, and arc-tanget functions aren't suited as well as periodic sine/cosine for straight 
minimax polynomial approximation, so they provide an option for fast(er)/slow(er) versions.

Optimizer notes  
There are also fast implementations using FMA instruction set to speed up polynom 
computation and reduce rounding error. And there is also a faster sin-cos function.
## Dependencies
1. A C++ compiler that supports C++20 standart.
The following compilers should work:

  * [gcc 10+](https://gcc.gnu.org/)

  * [clang 10+](https://clang.llvm.org/)

2. [CMake 3.12+](https://cmake.org/)
## Usage
In this section will be decribed the process of building, installing this project as a 
library so it can be linked to another project. Also there are some "before you start" notes.
### Build
Target platforms are Windows and Linux. You can build main target via CMake.
For example, from build directory:
```
 $ cmake %PATH to the main CMakeLists.txt% -DCMAKE_CXX_COMPILER:STRING=/usr/bin/clang++-11 -DCMAKE_BUILD_TYPE:String=Release
 $ make
```
### Install
After done building (from build directory) rebuild specifying install path:
```
 $ cmake %PATH to the main CMakeLists.txt% -DCMAKE_INSTALL_PREFIX=%INSTALL_PATH%
 $ cmake --build . --config Release --target install
```
Then to link the built library to your project with cmake use the following cmake lines:
```
find_package(geometrix CONFIG REQUIRED)
target_link_libraries(example_project geometrix::geometrix)
```
### Notes on enabling optimizations
There is an option present at runtime: to switch the implementation of several functions 
to use CPU intrinsics. To enable the optimizations call static function"init" 
in the "Optimizer" class when initializing your application.

## Testing
Tests contain unit-tests and benchmarks, and are run via CMake's CTest.  
See [TEST README](tests/README.md) for more detailed explanation of usage.

There are:
* accuracy tests, that check the specified errors constrains for trigonometry utilities
* unit-tests for linear algebra
* benchmark for trigonometry utilities, which assesses accuracy and performance, compared to stdlib   

### Building the tests
Tests should be built in debug mode for assertions to work.

Example:
```
 $ mkdir build
 $ cd build
 $ cmake %PATH to the main CMakeLists.txt% -DCMAKE_CXX_COMPILER:STRING=/usr/bin/clang++-11 -DCMAKE_BUILD_TYPE:String=Debug
 $ make
```

### Running the tests
To run built tests:
```
 $ make CTEST_OUTPUT_ON_FAILURE=1  test
```
or use ctest command:
```
 $ ctest --output-on-failure
```

## TODO:
* Finish up linear algebra and geometry unit-tests
* Add TODOs

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
