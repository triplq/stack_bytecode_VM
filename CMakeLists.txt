cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
set (CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(vm)
find_package(Boost 1.69 REQUIRED)
add_compile_options(-fsanitize=address,undefined -g -O1)
add_link_options(-fsanitize=address,undefined)
add_library(vm src/vm.cpp)
target_include_directories(vm PUBLIC src)
target_link_libraries(vm PUBLIC Boost::headers)
target_link_options(vm PRIVATE 
                    -Wl, --no-undefined
                    -Wl, --as-needed)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.tar.gz
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(tests tests/vm_test.cpp)
target_link_libraries(tests vm GTest::gmock_main)
include(GoogleTest)
gtest_discover_tests(tests)