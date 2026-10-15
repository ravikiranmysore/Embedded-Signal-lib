cmake_minimum_required(VERSION 3.16)
project(sigproc VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(SIGPROC_BUILD_TESTS "Build the unit tests" ON)
option(SIGPROC_BUILD_EXAMPLES "Build the demo program" ON)
option(SIGPROC_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)
option(SIGPROC_COVERAGE "Measure test coverage (GCC or Clang)" OFF)
option(SIGPROC_SANITIZE "Build with AddressSanitizer and UBSan" OFF)

# The library is header-only.
add_library(sigproc INTERFACE)
add_library(sigproc::sigproc ALIAS sigproc)
target_include_directories(sigproc INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_compile_features(sigproc INTERFACE cxx_std_17)

# Warning, coverage and sanitizer settings for our own targets only.
function(sigproc_apply_options target)
  if(MSVC)
    target_compile_options(${target} PRIVATE /W4)
    if(SIGPROC_WARNINGS_AS_ERRORS)
      target_compile_options(${target} PRIVATE /WX)
    endif()
  else()
    target_compile_options(${target} PRIVATE
      -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow)
    if(SIGPROC_WARNINGS_AS_ERRORS)
      target_compile_options(${target} PRIVATE -Werror)
    endif()
    if(SIGPROC_COVERAGE)
      target_compile_options(${target} PRIVATE --coverage -O0 -g)
      target_link_options(${target} PRIVATE --coverage)
    endif()
    if(SIGPROC_SANITIZE)
      target_compile_options(${target} PRIVATE
        -fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=all)
      target_link_options(${target} PRIVATE -fsanitize=address,undefined)
    endif()
  endif()
endfunction()

if(SIGPROC_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

if(SIGPROC_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
