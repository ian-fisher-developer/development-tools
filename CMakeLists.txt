cmake_minimum_required(VERSION 3.14)

include(cmake/Include.cmake)

project(statistics)

add_subdirectory(${PROJECT_SOURCE_DIR}/src src)

option(STATISTICS_BUILD_TESTS "Build the statistics unit-test program" OFF)
if(STATISTICS_BUILD_TESTS)
    add_subdirectory(${PROJECT_SOURCE_DIR}/tst tst)
endif()

option(STATISTICS_BUILD_EXAMPLES "Build the statistics example programs" OFF)
if(STATISTICS_BUILD_EXAMPLES)
    add_subdirectory(${PROJECT_SOURCE_DIR}/examples examples)
endif()
