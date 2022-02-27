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
target_compile_features(project_options INTERFACE cxx_std_20)

# for clang and gcc add -mfma and for msvc /fp:fast
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options("-march=native")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options("/fp:fast")
endif()


# Build tests
option(ENABLE_TESTING "Enable Test Builds" ON)

if(ENABLE_TESTING)
  message(
    "Building Tests."
  )
  add_subdirectory(tests)
  add_test(SinCosTest tests/SinCosTest 1)
  add_test(MatrixTest tests/MatrixTest 2)
  add_test(OptimizerTest tests/OptimizerTest 3)
  add_test(TrigBenchmark tests/TrigBenchmark 4)
  enable_testing()
endif()


# Install library
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
