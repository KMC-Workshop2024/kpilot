cmake_minimum_required(VERSION 3.16)
project(kpilot_velocity_tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

add_executable(test_velocity_predictor
    test_velocity_predictor.cpp
    ../src/velocity_predictor.cpp
)
target_include_directories(test_velocity_predictor PRIVATE ../include)

add_test(NAME velocity_predictor COMMAND test_velocity_predictor)
