#!/usr/bin/env sh

set -xe

OUT="cw_tree.out"
GFLAGS="-Wall -Wextra -Wswitch-enum -std=c++17"
DFLAGS="-ggdb -fsanitize=address -fsanitize=undefined"
RFLAGS="-O2"
VARFLAGS="-DTHREAD_PAUSE_MS=1000 -DTHREAD_GENERAL_MS=1"
CFLAGS="$GFLAGS $VARFLAGS $DFLAGS"
LIBS="-lraylib -lm"

build() {
    c++ $CFLAGS -o $OUT src/node.cpp src/state.cpp src/worker.cpp src/main.cpp $LIBS
}

if [ "$1" = "run" ]
then
    build && ./$OUT
elif [ "$1" = "release" ]
then
    CFLAGS="$GFLAGS $VARFLAGS $RFLAGS"
    build;
else
    build;
fi
