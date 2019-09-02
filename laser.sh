#!/bin/bash

TESTAPP_EXECUTABLE=testapp
GTEST_EXECUTABLE=run_gtest
CMAKE_EXE=cmake

build_release() {
    mkdir -p cmake-build-release
    cd cmake-build-release
    $CMAKE_EXE -DCMAKE_BUILD_TYPE=Release ..
    make # placeholder, should build L++ library
    cd ..
}

build_testapp() {
    mkdir -p cmake-build-debug
    cd cmake-build-debug
    $CMAKE_EXE -DCMAKE_BUILD_TYPE=Debug ..
    make VERBOSE=1 testapp
    cd ..
}

build_gtest() {
    mkdir -p cmake-build-debug
    cd cmake-build-debug
    $CMAKE_EXE -DCMAKE_BUILD_TYPE=Debug ..
    make run_gtest
    cd ..
}

clean_up () {
    rm -rf cmake-build-debug
    rm -rf cmake-build-release
}

run_testapp () {
    build_testapp
    cmake-build-debug/$TESTAPP_EXECUTABLE
}

debug_testapp () {
    build_testapp
    gdb cmake-build-debug/$TESTAPP_EXECUTABLE --tui
}

run_gtest () {
    build_gtest
    cmake-build-debug/$GTEST_EXECUTABLE
}

print_help () {
    echo "Usage: laser.sh [b c d h r t]"
    echo "b: build Laser++"
    echo "c: Clean project"
    echo "d: Debug Test App using GDB"
    echo "h: Print help"
    echo "r: Run Test App"
    echo "t: run all testcases"
    echo " "
}
if [ "$IS_DAS5" = "1" ]; then
    echo 'Running on DAS-5'
    export CMAKE_ROOT=~/tools/cmake/
    CMAKE_EXE=$CMAKE_ROOT/bin/cmake
    export CC=~/tools/llvm/bin/clang
    export CXX=~/tools/llvm/bin/clang++
    export PATH=~/tools/llvm/bin:$PATH
fi

if [ $# -eq 0 ]; then 
	print_help 
elif [ $1 = "b" ]; then
    build_release
elif [ $1 = "br" ]; then
    build_testapp
elif [ $1 = "bt" ]; then
    build_gtest
elif [ $1 = "c" ]; then
    clean_up
elif [ $1 = "d" ]; then
    debug_testapp
elif [ $1 = "h" ]; then
    print_help
elif [ $1 = "r" ]; then
    run_testapp
elif [ $1 = "t" ]; then
    run_gtest
else
	print_help
fi

#ln -s cmake-build-debug/compile_commands.json
