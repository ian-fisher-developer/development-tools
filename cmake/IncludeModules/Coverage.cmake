# Add helpers for generating a coverage report
#
# Targets:
#
# * coverage - Link this interface library to targets that you want to be able to generate coverage
#   ports for. Note, this will only apply to objects that are built as part of that target, so if
#   you want to get a coverage report including your executable and a static lib that it links, you
#   need to link this to both the static lib target and the exectuable targets
#
# Functions:
#
# * add_generate_coverage_report_target(prefix [args...]) - Create a new target called
#   [prefix]_generate_coverage_report which can be built to invoke gcovr. Any additional args passed
#   will be passed to gcovr - use this to specify filter and exclude options to tailor the report to
#   the correct source files. Example: add_coverage_report_target(tachyon --filter
#   ${CMAKE_SOURCE_DIR}/src --exclude ${CMAKE_SOURCE_DIR}/src/tests)
add_library(coverage INTERFACE)

# Link against the `coverage` target to add the appropriate compile options to your target to
# generate coverage report files
#
# Only modern GCC supports requring absolute paths in the gcno files
#
# /todo - figure out what version of GCC added -fprofile-abs-path and lock the VERSION_GREATER check
# to that instead of 5
target_compile_options(
    coverage
    INTERFACE
        --coverage
        $<$<AND:$<CXX_COMPILER_ID:GNU>,$<VERSION_GREATER:CXX_COMPILER_VERSION,5>>:-fprofile-abs-path>
)
target_link_options(coverage INTERFACE --coverage)

# Disable assert calls in code being instrumented because we should never hit the failure condition
# and don't want to be testing it
target_compile_definitions(coverage INTERFACE NDEBUG)

function(add_generate_coverage_report_target target)

    find_package(GCov REQUIRED)
    find_package(GCovr 4.2 REQUIRED)

    add_custom_target(
        ${target}_coverage_report
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMAND mkdir -p ${target}_coverage_report
        COMMAND
            ${GCovr_EXECUTABLE} --root ${CMAKE_SOURCE_DIR} ${ARGN} --branches
            --exclude-throw-branches --exclude-unreachable-branches --print-summary --xml-pretty
            --xml ${target}_coverage_report/coverage_report.xml --html-details
            ${target}_coverage_report/coverage_report.html --gcov-executable "${GCov_COMMAND}"
            ${CMAKE_BINARY_DIR}
        BYPRODUCTS ${target}_coverage_report/coverage_report.xml
                   ${target}_coverage_report/coverage_report.html
    )
endfunction()
