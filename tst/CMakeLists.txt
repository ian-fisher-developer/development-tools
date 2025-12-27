add_subdirectory(googletest googletest)

add_executable(
    ${PROJECT_NAME}_test StatisticsCalculatorTest.cpp StatisticsReportsHelpersTest.cpp
                         StatisticsReportTest.cpp StatisticsUtilitiesTest.cpp
)
target_include_directories(${PROJECT_NAME}_test PRIVATE ${PROJECT_SOURCE_DIR}/src/lib)
target_link_libraries(${PROJECT_NAME}_test PRIVATE gtest gtest_main ${PROJECT_NAME})

add_custom_target(
    ${PROJECT_NAME}_test_run
    COMMAND ${CMAKE_BINARY_DIR}/tst/${PROJECT_NAME}_test
            --gtest_output=xml:"${CMAKE_BINARY_DIR}/${PROJECT_NAME}_xunit.xml"
    DEPENDS ${PROJECT_NAME}_test
)

add_executable(${PROJECT_NAME}_stress_test test_data/StressData.cpp StatisticsStressTest.cpp)
target_include_directories(${PROJECT_NAME}_stress_test PRIVATE ${PROJECT_SOURCE_DIR}/src/lib)
target_link_libraries(${PROJECT_NAME}_stress_test PRIVATE gtest gtest_main ${PROJECT_NAME})

add_executable(
    ${PROJECT_NAME}_extreme_stress_test test_data/StressData.cpp StatisticsExtremeStressTest.cpp
)
target_include_directories(
    ${PROJECT_NAME}_extreme_stress_test PRIVATE ${PROJECT_SOURCE_DIR}/src/lib
)
target_link_libraries(${PROJECT_NAME}_extreme_stress_test PRIVATE gtest gtest_main ${PROJECT_NAME})

option(STATISTICS_GENERATE_COVERAGE_REPORT
       "Generate code coverage reports from the statistics_test program run" OFF
)
if(STATISTICS_GENERATE_COVERAGE_REPORT)
    include(Coverage)
    target_link_libraries(${PROJECT_NAME} PUBLIC coverage)
    add_generate_coverage_report_target(${PROJECT_NAME} --filter ${PROJECT_SOURCE_DIR}/src)
    add_dependencies(${PROJECT_NAME}_coverage_report ${PROJECT_NAME}_test_run)
endif()
