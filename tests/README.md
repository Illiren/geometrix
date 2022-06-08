# Detailed information on the testing process
## How to write tests
Add source file
Add th test case to the main CMakeLists.txt under "# Build tests" section, example:
```
add_test(test_name tests/test_target_name)
```
And add the corresponding target inside tests/CMakeLists.txt, example:
```
add_executable(test_target_name unit_tests/trigonometry/test_target_name.cpp)
target_link_libraries(test_target_name PRIVATE project_options)
```
CTest considers the test result successful if main() program return zero code and no
exceptions were thrown, so you can use assert expressions. 
## How to build tests
In order to build you must specify corresponding CMake parameters, when configuring the 
project, and use a Debug build to handle exceptions. There are ENABLE_BENCH and 
DENABLE_TESTING variables.

Example of how to build all tests and benchmarks from build directory:
```
 $ cmake .. -DCMAKE_CXX_COMPILER:STRING=/usr/bin/clang++-11 -DENABLE_BENCH=ON -DENABLE_TESTING=ON -DCMAKE_BUILD_TYPE=Debug
```
## How to run tests
In order to see where test has failed use "--output-on-failure" option:
```
 $ ctest --output-on-failure
```
And in order to see all the output of the test cases use "-V" option.  
In order to output bechmark results to a file use "-O" and "-V" options:
```
 $ ctest -O "file_path" -V
```
