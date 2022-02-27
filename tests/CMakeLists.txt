add_executable(TrigBenchmark utility.hpp Trigonometry/trigonometrixBench.cpp)
target_link_libraries(TrigBenchmark PRIVATE project_options)

add_executable(SinCosTest Trigonometry/sinCosTest.cpp)
target_link_libraries(SinCosTest PRIVATE project_options)

add_executable(MatrixTest Matrix/matrixTests.cpp)
target_link_libraries(MatrixTest PRIVATE project_options)

add_executable(OptimizerTest optimizerTests.cpp)
target_link_libraries(OptimizerTest PRIVATE project_options)

include(CTest)
