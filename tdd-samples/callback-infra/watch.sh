# A shell-wrapper to trigger build and execute whenever any file in this tree changes. 
# Use during development for rapid feedback.

REPO_ROOT=$(git rev-parse --show-toplevel)
WATCH_DIR=$PWD
bash -c "mkdir -p $REPO_ROOT/build && cd $REPO_ROOT/build && cmake $WATCH_DIR"
$REPO_ROOT/bin/rerun -c -v bash -c "cd $REPO_ROOT/build && make && make CTEST_OUTPUT_ON_FAILURE=1 test"
