#!/bin/bash

BUILD_DIR=build

RUN_MAIN=false
RUN_TESTS=false
CLEAN_BUILD=false
VERBOSE_CMAKE=false
VERBOSE_MAKE=false

function exit_on_failure() {
    if [ $? -ne 0 ]; then
        printf "${RED}X\n"
        echo "Error: $1 failed. Exiting."
        exit 1
    fi
}

# Parse flags
while getopts "crtCM" flag; do
    case $flag in
        c) CLEAN_BUILD=true ;;
        r) RUN_MAIN=true ;;
        t) RUN_TESTS=true ;;
        C) VERBOSE_CMAKE=true ;;
        M) VERBOSE_MAKE=true ;;
        *)
            echo "Usage: $0 [-r] [-t] [-c] [-C] [-M] [ARGS...]"
            echo "  -r: Run main"
            echo "  -t: Run tests"
            echo "  -c: Clean build directory"
            echo "  -C: Verbose output for cmake"
            echo "  -M: Verbose output for make"
            echo "  ARGS: Arguments to pass to the program"
            exit 1
            ;;
    esac
done

shift $((OPTIND - 1))

ADDITIONAL_ARGS="$@"

if [ "$CLEAN_BUILD" = true ]; then
    echo "Cleaning build directory"
    rm -rf $BUILD_DIR
    exit_on_failure "Clean build directory"
fi

mkdir -p $BUILD_DIR

cd $BUILD_DIR

if [ "$VERBOSE_CMAKE" = true ]; then
    cmake -S ..
else
    cmake -S .. &> /dev/null
fi

exit_on_failure "Cmake"

if [ "$VERBOSE_MAKE" = true ]; then
    make
else
    make &> /dev/null
fi

exit_on_failure "Make"

if [ "$RUN_TESTS" = true ]; then
    ctest
    exit_on_failure "Tests"
fi

if [ "$RUN_MAIN" = true ]; then
    ./Cpplexer $ADDITIONAL_ARGS
    exit_on_failure "Main"
fi

