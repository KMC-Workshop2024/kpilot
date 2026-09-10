cmake_minimum_required(VERSION 3.25)
project(kpilot_tests LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Test Network)

enable_testing()

qt_add_executable(test_server_endpoint
    test_server_endpoint.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/network/server_endpoint.cpp
)
target_include_directories(test_server_endpoint PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)
target_link_libraries(test_server_endpoint PRIVATE Qt6::Core Qt6::Test Qt6::Network)

add_test(NAME server_endpoint COMMAND test_server_endpoint)
