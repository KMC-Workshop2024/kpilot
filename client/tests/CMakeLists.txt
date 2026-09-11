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
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/network/network_log_sanitizer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/fsd/pdu/pdu_add_pilot.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/fsd/pdu/pdu_base.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/fsd/serializer.cpp
)
target_include_directories(test_server_endpoint PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)
target_link_libraries(test_server_endpoint PRIVATE Qt6::Core Qt6::Test Qt6::Network)

add_test(NAME server_endpoint COMMAND test_server_endpoint)

file(GLOB FSD_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/fsd/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/fsd/pdu/*.cpp
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../dependencies/vatsim-auth
                 ${CMAKE_CURRENT_BINARY_DIR}/vatsim-auth)

qt_add_executable(test_fsd_private
    test_fsd_private.cpp
    ${FSD_SOURCES}
)
target_include_directories(test_fsd_private PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)
target_link_libraries(test_fsd_private PRIVATE Qt6::Core Qt6::Test Qt6::Network vatsim-auth)

add_test(NAME fsd_private COMMAND test_fsd_private)

qt_add_executable(test_msfs2024_data
    test_msfs2024_data.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/simulator/msfs2024/own_aircraft_data.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/simulator/msfs2024/msfs2024_backend.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/simulator/msfs2024/simconnect_symbols.cpp
)
target_include_directories(test_msfs2024_data PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)
target_link_libraries(test_msfs2024_data PRIVATE Qt6::Core Qt6::Test)

add_test(NAME msfs2024_data COMMAND test_msfs2024_data)

add_test(
    NAME private_branding
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_SOURCE_DIR}/test_private_branding.cmake
)

add_test(
    NAME installer_plugin_layout
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_SOURCE_DIR}/test_installer_plugin_layout.cmake
)

add_test(
    NAME xplane_coexistence
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_SOURCE_DIR}/test_xplane_coexistence.cmake
)
