# An Infrastructure for Timed Callbacks

This project is about building a solution to a larger problem using the outside-in classicist TDD approach in C++. The aim of the project is to build a service that can provide timed callbacks to clients.

## Problem Statement

Build a mechanism for setting-up timed callbacks.

1. The system shall allow clients to register and de-register callbacks.
2. The system shall invoke the callbacks at the registered time-intervals (between registration and de-registration)
3. The system shall have a configurable clock-tick that will influence callback period granularity (shall be in multiple of clock-ticks) and polling performance of the system (a system with a larger clock-tick shall use not more CPU/ OS system calls than a system with a smaller clock tick).

### Things to do

These are things to do for later. Feel free to start working on any of these after submitting it as an issue & assigning it to yourself.

* Add Bazel workspace + build with fetching for any external dependencies
* Move doctest out of the code-base (and as an external dependency)

## Repo layout
Following is the proposed package layout.



| item       | description                   | Remarks                                                      |
| ---------- | ----------------------------- | ------------------------------------------------------------ |
| `acc`      | contains acceptance tests.    | Each cpp file is an individual test which is fully self-contained (and compiled and executed independently). |
| `include`  | contains public header files. | All tests depend on this.                                    |
| `src`      | contains the solution         | All production code lives here (and inside `include`)        |
| `watch.sh` | wrapper script for TDD        | This script watches the source-tree and runs the test targets whenever any file is saved (even README.md). |



## CMake Setup

This project uses [**CMake**](https://cmake.org/download/) and [**doctest**](https://github.com/onqtam/doctest)

You can edit any CMakeLists.txt file to set the targeted C++ Standard (Default is C++11) in the variable _CXX_STANDARD_.

#### Requirements

* C++ Compiler with possibily support to modern standards (e.g. gcc >= 5, clang >= 3.8, Visual Studio >= 2013)
* CMake

#### Unix-Like Setup

To run the project, just open a terminal (on Linux/Mac) and run the following commands:

```bash
cd /path/to/this/project
mkdir build && cd build
cmake ../acc
```

To run the tests just run:

```bash
make
make test
```

To run the tests with the test outputs displayed on the console, just run:
```bash
make && make CTEST_OUTPUT_ON_FAILURE=1 test
```
Each top-level directory is its own project including the necessary source and include files and adding the relevant test targets. Idea is that these CMake configs provide a test target that can be invoked to run the tests at that level.

The included watch.sh script provides a way to continuously monitor the directory for any changes and automatically re-build and run tests when any files in the directory tree is modified on disk.

#### Windows

If you have Visual Studio, just launch the CMake Gui tool and select the project source folder and a build directory and which version of Visual Studio you want to target.

After pressing the *Generate*  button, you can open the Visual Studio *.sln file generated in the build directory you have previously selected.


## Tips for Test-driven development in this repo

The watch.sh can be run to continuously monitor the project tree for changes and re-run tests when it detects any change.

The CMAKE_SUBDIR option controls which tests are built and run. This is useful, e.g., when focussing on a Unit-level TDD for a specific class for a while (several cycles of red-green-refactor) and then switch to an acceptance test that uses this class and make it pass.

Focussing on a smaller level (at exclusion of higher-level tests that continue to fail) is necessary to reach the green phase at Unit-level or Integration-level even when the acceptance tests fail - which will be the case until the feature is completely developed!

So, set the CMAKE_SUBDIR to an appropriate value and run watch.sh.

    Note: Although watch.sh can detect changes to itself it doesn't re-configure CMake when it detects a change (this is to make the feedback loop faster). Same is the case with changes to any CMakeLists.txt files.

So, whenever you make a change to any CMakeLists.txt file or the watch.sh script, terminate the running instance and re-launch it.
