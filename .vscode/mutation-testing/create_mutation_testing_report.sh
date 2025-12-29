#!/bin/bash


# Allow this script to be called from anywhere.

mutation_testing_dir=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )


# Check this script finds the top directory.

top_dir=$(dirname $(dirname "$mutation_testing_dir"))
if [[ ! -d $top_dir/src ]] || [[ ! -d $top_dir/tst ]] ; then
  echo Error in $(basename $0): This script assumes it is two directories down from the top directory
  echo Error in $(basename $0): Testing the assumption: $top_dir does not look like the top directory
  exit 1
fi


# Document this script.

build_dir="$top_dir"/build/build-mutation-testing-reports

description () {
  echo ""
  echo "Description:"
  echo "  Create a local mutation testing report for a unit test program."
  echo "  The build configuration specifies the required flags for mutation testing."
  echo "  It avoids interfering with the developer's current configurations by"
  echo "  using its own build directory:"
  echo "    $build_dir"
}

diffs=no-diff-filter
timeout=120000 # 2 minutes
workers=`nproc`

usage () {
  echo ""
  echo "Usage:"
  echo "  $(basename $0) [-h] [-d DIFFS] [-t TIMEOUT] [-w WORKERS] build_target"
  echo "  Options:"
  echo "    -h          Print the description, usage, and examples."
  echo "    -d DIFFS    Reduce analysis to just the code changes. Specify one of:"
  echo "                branch-diffs, unstaged-diffs, staged-and-unstaged-diffs."
  echo "                Default: $diffs"
  echo "    -t TIMEOUT  Mull time limit for each test, in milliseconds."
  echo "                Default: $timeout"
  echo "    -w WORKERS  Number of parallel Mull runner workers."
  echo "                For N mutants the test program is run N times."
  echo "                Multiple workers reduce the analysis time."
  echo "                Default (from nproc): $workers"
}

examples () {
  echo ""
  echo "Examples:"
  echo "  $(basename $0) -h"
  echo "  $(basename $0) statistics_test"
  echo "  $(basename $0) -d branch-diffs statistics_test"
}

help () {
  description
  usage
  examples
}


# Query the args for optional items, and then for required items.

while getopts "hd:t:w:" option; do
  case "$option" in
    h)
      help
      exit 0
      ;;
    d)
      diffs="$OPTARG"
      ;;
    t)
      timeout=$OPTARG
      ;;
    w)
      workers=$OPTARG
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


# Specify the Mull configuration files and check they exist.

mull_build_config="$mutation_testing_dir"/config/build.yml
mull_runner_diff_config="$mutation_testing_dir"/config/diffs/"$diffs".yml
mull_runner_target_config="$mutation_testing_dir"/config/targets/"$build_target".yml

function check_mull_config_file_exists {
  mull_config_file="$1"
  if [ ! -f "$mull_config_file" ]
  then
    echo ""
    echo "Error in $(basename $0): could not find Mull configuration file $mull_config_file"
    usage
    exit 1
  fi
}
check_mull_config_file_exists "$mull_build_config"
check_mull_config_file_exists "$mull_runner_diff_config"
check_mull_config_file_exists "$mull_runner_target_config"


# Prepare for the mutation testing build:
#  - note if any build artifacts already exist--useful summary info
#  - export the Mull build configuration as MULL_CONFIG for mull-ir-frontend

test_program_dir="$build_dir"/bin
test_program="$test_program_dir"/"$build_target"

build_scope="Fresh configure and Clang build"
[[ -d "$build_dir" ]] && build_scope="Clean Clang build"
[[ -f "$test_program" ]] && build_scope="Incremental Clang build"

export MULL_CONFIG="$mull_build_config"


# Make the build directory the working directory--the mutation analysis dumps
# cores for crashing mutants along with other files that should be contained.
#
# Note that the working directory affects the gitProjectRoot setting in
# the config/diffs files, used by this script's DIFFS option.
# Be sure to match any changes in this working directory with the
# gitProjectRoot paths in the config/diffs files.

mkdir -p "$build_dir"
cd "$build_dir"


# Configure the RIOT build for mutation testing and build the test target.

export ARCH="x86"

llvm_version=14
mull_instruments="-fexperimental-new-pass-manager -fpass-plugin=/usr/lib/mull-ir-frontend-$llvm_version \
                  -g -grecord-command-line -fprofile-instr-generate -O0 -fcoverage-mapping"

cmake -S "$top_dir" -B "$build_dir" -G "Ninja" \
  -DSTATISTICS_BUILD_TESTS="ON" \
  -DCMAKE_BUILD_TYPE="Debug" \
  -DCMAKE_CXX_COMPILER="/usr/bin/clang++-$llvm_version" \
  -DCMAKE_CXX_FLAGS="$mull_instruments" \
  || exit 1

cmake --build "$build_dir" --target "$build_target" \
  || exit 1


# Check the test program is good for Mull analysis:
# - it exists in the expected location
# - it runs without errors, outside the CMake build

if [ ! -f "$test_program" ]
then
  echo ""
  echo Error in $(basename $0): could not find test program "$build_target" in build directory "$test_program_dir"
  exit 1
fi

echo ""
echo Info from $(basename $0): Check that "$test_program" runs without errors, outside the CMake build...
echo ""
"$test_program" || exit 1
echo ""
echo Info from $(basename $0): "$test_program" ran successfully outside the CMake build


# Prepare for the Mull analysis:
# - define a report file for the analysis output and summary
# - create a combined config file from the "diffs" and "targets" configs
# - export the combined configuration as MULL_CONFIG for mull-runner

echo ""
echo Info from $(basename $0): Starting the Mull analysis...
echo ""

reports_dir="$build_dir"/reports
mkdir -p "$reports_dir"
report="$reports_dir"/"$build_target"_"$diffs".txt

mull_runner_config="$reports_dir"/"$build_target"_"$diffs".yml
cat "$mull_runner_diff_config" "$mull_runner_target_config" > "$mull_runner_config"
check_mull_config_file_exists "$mull_runner_config"

export MULL_CONFIG="$mull_runner_config"


# Run the Mull analysis.

mull-runner-$llvm_version \
    --ld-search-path /usr/lib/x86_64-linux-gnu \
    --timeout $timeout \
    --workers $workers \
    "$test_program" \
    | tee "$report"


# Print a summary and append it to the report file.

summary=( \
  "" \
  "==================== $(basename $0) run details ====================" \
  "Date: $(date)" \
  "Top directory: $top_dir" \
  "Current branch and commit: $(git branch --show-current) $(git rev-parse HEAD)" \
  "Steps:" \
  "  - $build_scope of target $build_target" \
  "  - Mull $diffs mutation analysis of test program $build_target" \
  "Total build and analysis time: $SECONDS seconds" \
  "==================== $(basename $0) run details ===================="
)

for line in "${summary[@]}"; do
  echo "$line" | tee -a "$report"
done

echo ""
echo ==================== $(basename $0) run results ====================
echo Mutation testing report ready for "$build_target":
echo "  - $report"
echo ==================== $(basename $0) run results ====================
echo ""
