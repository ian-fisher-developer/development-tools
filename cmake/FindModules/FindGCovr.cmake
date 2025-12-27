# Find GCovr
#
# Sets the following variables
#
# * GCovr_FOUND
# * GCovr_EXECUTABLE
# * GCovr_VERSION
if(NOT GCovr_FOUND)
    find_program(GCovr_EXECUTABLE gcovr)

    if(GCovr_EXECUTABLE)
        execute_process(COMMAND ${GCovr_EXECUTABLE} --version OUTPUT_VARIABLE gcovr_version_output)
        string(REGEX MATCH "gcovr +([0-9]+[.][0-9]+)" _ "${gcovr_version_output}")
        set(GCovr_VERSION
            "${CMAKE_MATCH_1}"
            CACHE STRING "" FORCE
        )
        mark_as_advanced(GCovr_VERSION)
    endif()

    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(
        GCovr
        REQUIRED_VARS GCovr_EXECUTABLE
        VERSION_VAR GCovr_VERSION
    )
endif()
