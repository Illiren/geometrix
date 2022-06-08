cmake_minimum_required(VERSION 3.12)

set(project_name geometrix)
project(${project_name}
    LANGUAGES CXX
    VERSION 0.0.7
    )

add_library(project_options INTERFACE)

# "Build" the library
add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
    
# Set compiler options
target_compile_features(project_options INTERFACE cxx_std_20)
# for clang and gcc add -mfma and for msvc /fp:fast
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options("-march=native")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options("/fp:fast")
endif()
if (CMAKE_BUILD_TYPE MATCHES "Release")
	add_compile_options(-O3 -DNDEBUG)
endif()

# Build tests
option(ENABLE_TESTING "Enable Test Builds" ON)
option(ENABLE_BENCH "Enable Benchmark Builds" OFF)

if(ENABLE_TESTING OR ENABLE_BENCH)
  add_subdirectory(tests)
  enable_testing()
endif()
if(ENABLE_TESTING)
  message("Building Unit Tests.")
  add_test(Sin/Cos_Unit_Tests tests/test_sin_cos)
  add_test(Tangent/ArcTangent_Unit_Tests tests/test_tan_atan)
  add_test(ArcSin/ArcCos_Unit_Tests tests/test_asin_acos)
  add_test(Matrix_Unit_Tests tests/test_matrix)
  add_test(Optimizer_Test tests/test_optimizer)
endif()
if(ENABLE_BENCH)
  message("Building Benchmarks.")
  add_test(Trigonometry_Benchmark tests/bench_trigonometrix)
endif()

# Install the library
include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}_Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
include(CMakePackageConfigHelpers)
write_basic_package_version_file("${PROJECT_NAME}ConfigVersion.cmake"
                                 VERSION ${PROJECT_VERSION}
                                 COMPATIBILITY SameMajorVersion)
if(NOT INCLUDE_INSTALL_DIR)
  set(INCLUDE_INSTALL_DIR include)
endif()

configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
  PATH_VARS INCLUDE_INSTALL_DIR)
install(EXPORT ${PROJECT_NAME}_Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)
install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
              "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION
    ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
