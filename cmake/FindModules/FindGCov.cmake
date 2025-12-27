# Find GCov
#
# This will find the gcov executable that matches the CMAKE_CXX_COMPILER_ID. This is not compatible
# with GCC < 5
#
# Sets the following variables
#
# * GCov_FOUND
# * GCov_EXECUTABLE
# * GCov_COMMAND
#
# GCov_COMMAND will differ from GCov_EXECUTABLE when the Clang toolchain is in use. llvm-cov needs
# an extra argument to put it in gcov mode.
if(NOT GCov_FOUND)
    if(CMAKE_CXX_COMPILER_ID STREQUAL GNU)
        set(gcov_base gcov)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL Clang)
        set(gcov_base llvm-cov)
    else()
        message(FATAL_ERROR "Unable to determine gcov executable for ${CMAKE_CXX_COMPILER_ID}")
    endif()

    # Get compiler major version Note - might not work with gcc < 5
    string(REPLACE "." ";" version_split ${CMAKE_CXX_COMPILER_VERSION})
    list(GET version_split 0 compiler_major_version)

    set(gcov_exe "${gcov_base}-${compiler_major_version}")
    find_program(GCov_EXECUTABLE ${gcov_exe})

    if(CMAKE_CXX_COMPILER_ID STREQUAL GNU)
        set(GCov_COMMAND
            "${GCov_EXECUTABLE}"
            CACHE STRING ""
        )
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL Clang)
        set(GCov_COMMAND
            "${GCov_EXECUTABLE} gcov"
            CACHE STRING ""
        )
    endif()
    mark_as_advanced(GCov_COMMAND)

    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(GCov REQUIRED_VARS GCov_EXECUTABLE GCov_COMMAND)
endif()
