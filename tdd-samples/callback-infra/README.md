# Introduction

This project is about building a solution to a larger problem using the outside-in classicist TDD approach in C++. The aim of the project is to build a service that can provide timed callbacks to clients.

# Problem Statement

Build a mechanism for setting-up timed callbacks.

1. The system shall allow clients to register and de-register callbacks.
2. The system shall invoke the callbacks at the registered time-intervals (between registration and de-registration)
3. The system shall have a configurable clock-tick that will influence callback period granularity (shall be in multiple of clock-ticks) and polling performance of the system (a system with a larger clock-tick shall use not more CPU/ OS system calls than a system with a smaller clock tick).

# Things to do

These are things to do for later. Feel free to start working on any of these after submitting it as an issue & assigning it to yourself.

* Add Bazel workspace + build with fetching for any external dependencies
* Move doctest out of the code-base (and as an external dependency)



CMake Setup
==========

This is a simple bootstrap project for C++ using [**CMake**](https://cmake.org/download/) and [**doctest**](https://github.com/onqtam/doctest)

You can edit the CMakeLists.txt file to set the targeted C++ Standard (Default is C++11) in the variable _CXX_STANDARD_.

### Requirements

* C++ Compiler with possibily support to modern standards (e.g. gcc >= 5, clang >= 3.8, Visual Studio >= 2013)
* CMake

### Unix-Like Setup

To run the project, just open a terminal (on Linux/Mac) and run the following commands:

```
cd /path/to/this/project
mkdir build && cd build
cmake ..
```

To run the tests just run:

```
make
make test
```

To run the tests with the test outputs displayed on the console, just run:
```
make && make CTEST_OUTPUT_ON_FAILURE=1 test
```

The included watch.sh script provides a way to continuously monitor the directory for any changes and automatically re-build and run tests when any files in the directory tree is modified on disk.

### Windows

If you have Visual Studio, just launch the CMake Gui tool and select the project source folder and a build directory and which version of Visual Studio you want to target.

After pressing the *Generate*  button, you can open the Visual Studio *.sln file generated in the build directory you have previously selected.

