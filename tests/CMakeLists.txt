# Use GoogleTest from the system if it exists. Otherwise download it.
find_package(GTest QUIET)
if(NOT GTest_FOUND)
  include(FetchContent)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)
endif()

add_executable(sigproc_tests
  test_ring_buffer.cpp
  test_moving_average.cpp
  test_median_filter.cpp
  test_low_pass.cpp
  test_debouncer.cpp
  test_hysteresis.cpp
  test_pipeline.cpp)

target_link_libraries(sigproc_tests PRIVATE sigproc GTest::gtest_main)
sigproc_apply_options(sigproc_tests)

include(GoogleTest)
gtest_discover_tests(sigproc_tests)
