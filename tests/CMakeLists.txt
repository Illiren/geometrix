if(ENABLE_BENCH)
  add_executable(bench_trigonometrix benchmarks/bench_trigonometrix.cpp)
  target_link_libraries(bench_trigonometrix PRIVATE project_options)
endif()

if(ENABLE_TESTING)
  add_executable(test_sin_cos unit_tests/trigonometry/test_sin_cos.cpp)
  target_link_libraries(test_sin_cos PRIVATE project_options)

  add_executable(test_tan_atan unit_tests/trigonometry/test_tan_atan.cpp)
  target_link_libraries(test_tan_atan PRIVATE project_options)

  add_executable(test_asin_acos unit_tests/trigonometry/test_asin_acos.cpp)
  target_link_libraries(test_asin_acos PRIVATE project_options)

  add_executable(test_matrix unit_tests/matrix/test_matrix.cpp)
  target_link_libraries(test_matrix PRIVATE project_options)

  add_executable(test_optimizer unit_tests/test_optimizer.cpp)
  target_link_libraries(test_optimizer PRIVATE project_options)

  add_executable(test_trig_constexpr unit_tests/trigonometry/test_constexpr.cpp)
  target_link_libraries(test_trig_constexpr PRIVATE project_options)
endif()

include(CTest)
