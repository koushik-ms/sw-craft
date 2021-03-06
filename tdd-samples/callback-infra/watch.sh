# A shell-wrapper to trigger build and execute whenever any file in this tree changes.
# Use during development for rapid feedback.
#
# Usage:
#     watch.sh <CMakeLists.txt path> <target to run after make>

if [ $# -ne 1 ]; then
  echo 1>&2 "Usage: $0 <target to run after make>"
  exit 3
fi

CMAKE_SUBDIR="" # Change to `acc` or `int` to run diff tests
WATCH_DIR=$PWD

REPO_ROOT=$(git rev-parse --show-toplevel)
BS=build
bash -c "rm -rf $REPO_ROOT/$BS"
bash -c "mkdir $REPO_ROOT/$BS && cd $REPO_ROOT/$BS && cmake $WATCH_DIR/$CMAKE_SUBDIR"
$REPO_ROOT/bin/rerun -c -v bash -c "cd $REPO_ROOT/$BS && make && make CTEST_OUTPUT_ON_FAILURE=1 ${1}"
