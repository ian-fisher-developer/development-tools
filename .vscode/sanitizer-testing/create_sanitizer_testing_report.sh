#!/bin/bash


# Allow this script to be called from anywhere.

script_dir=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )


# Check this script finds the top directory.

top_dir=$(dirname $(dirname "$script_dir"))
if [[ ! -d $top_dir/src ]] || [[ ! -d $top_dir/tst ]] ; then
  echo Error in $(basename $0): This script assumes it is two directory down from the top directory
  echo Error in $(basename $0): Testing the assumption: $top_dir does not look like the top directory
  exit 1
fi


# Document this script.

build_dir="$top_dir"/build/build-sanitizer-testing-reports

description () {
  echo ""
  echo "Description:"
  echo "  Create a local code sanitizer report for a test program."
  echo "  The build configuration specifies the required flags for the sanitizer."
  echo "  It avoids interfering with the developer's current configurations by"
  echo "  using its own build directory:"
  echo "    $build_dir"
}

sanitizer_name=address

usage () {
  echo ""
  echo "Usage:"
  echo "  $(basename $0) [-h] [-s SANITIZER] build_target"
  echo "  Options:"
  echo "    -h              Print the description, usage, and examples."
  echo "    -s SANITIZER    Name of the code sanitizer. Specify one of:"
  echo "                    address (the AddressSanitizer)"
  echo "                    undefined (the UndefinedBehaviorSanitizer)"
  echo "                    Default: $sanitizer_name"
}

examples () {
  echo ""
  echo "Examples:"
  echo "  $(basename $0) -h"
  echo "  $(basename $0) statistics_test"
  echo "  $(basename $0) -s address statistics_test"
  echo "  $(basename $0) -s undefined statistics_test"
}

help () {
  description
  usage
  examples
}


# Query the args for optional items, and then for required items.

while getopts "hs:i:" option; do
  case "$option" in
    h)
      help
      exit 0
      ;;
    s)
      sanitizer_name="$OPTARG"
      ;;
    *)
      usage
      exit 1
      ;;
  esac
done

shift "$(($OPTIND -1))"

if [ -z $1 ]; then
  echo ""
  echo "Error in $(basename $0): no build target specified"
  usage
  exit 1
else
  build_target="$1"
fi


# Prepare for the code sanitizer builds:
#  - note if any build artifacts already exist--useful summary info

sanitizer_build_dir="$build_dir"/sanitizer-$sanitizer_name
sanitizer_reports_dir="$build_dir"/reports
sanitizer_report="$sanitizer_reports_dir"/"$build_target"_'$sanitizer_name'.txt

test_program_dir="$sanitizer_build_dir"/bin
test_program="$test_program_dir"/"$build_target"

build_scope="Fresh configure and build"
[[ -d "$sanitizer_build_dir" ]] && build_scope="Clean build"
[[ -f "$test_program" ]] && build_scope="Incremental build"

mkdir -p "$sanitizer_build_dir"
mkdir -p "$sanitizer_reports_dir"
cd "$sanitizer_build_dir"


# Configure the build with a code sanitizer and build the target.

export ARCH="x86"
export ASAN_OPTIONS="detect_leaks=1:log_path=$sanitizer_report"
export UBSAN_OPTIONS="print_stacktrace=1:log_path=$sanitizer_report"

sanitize="-fno-omit-frame-pointer -fsanitize=$sanitizer_name"

cmake -S "$top_dir" -B "$sanitizer_build_dir" -G "Ninja" \
  -DCMAKE_C_FLAGS="$sanitize" \
  -DCMAKE_CXX_FLAGS="$sanitize" \
  -DCMAKE_BUILD_TYPE=Debug \
  -DSTATISTICS_BUILD_TESTS="ON" \
  -DSTATISTICS_GENERATE_COVERAGE_REPORT="OFF" \
  || exit 1

cmake --build "$sanitizer_build_dir" --target "$build_target" \
  || exit 1


# Run the program to generate the report.

"$test_program" \
  || exit 1


echo ""
echo ==================== $(basename $0) run details ====================
echo Date: `date`
echo Statistics top directory: "$top_dir"
echo Current branch and commit: `git branch --show-current` `git rev-parse HEAD`
echo Steps:
echo "  - $build_scope of target $build_target with -fsanitize=$sanitizer_name"
echo Total build and report generation time: $SECONDS seconds
echo ==================== $(basename $0) run details ====================
echo ""
echo ==================== $(basename $0) run results ====================
echo Check the reports directory for a new "$build_target"_$sanitizer_name report
echo from this run. No new report means no problems were found:
echo "  ls -latr $sanitizer_reports_dir"
ls -latr $sanitizer_reports_dir
echo ==================== $(basename $0) run results ====================
echo ""
