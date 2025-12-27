add_library(${PROJECT_NAME} STATIC)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror)

target_sources(
    ${PROJECT_NAME}
    PRIVATE headers/stats/StatisticsCalculator.hpp
            headers/stats/StatisticsReport.hpp
            headers/stats/StatisticsUtilities.hpp
            lib/StatisticsCalculator.cpp
            lib/StatisticsReport.cpp
            lib/StatisticsReportsHelpers.cpp
            lib/StatisticsReportsHelpers.hpp
            lib/StatisticsUtilities.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC headers)
