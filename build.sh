#!/usr/bin/env sh

set -xe

OUT="cw_tree.out"
GFLAGS="-Wall -Wextra -Wswitch-enum -std=c++17"
DFLAGS="-ggdb -fsanitize=address -fsanitize=undefined"
RFLAGS="-O2"
CFLAGS="$GFLAGS $DFLAGS"
LIBS="-lraylib -lm"

build() {
    c++ $CFLAGS -o $OUT src/node.cpp src/state.cpp src/worker.cpp src/main.cpp $LIBS
}

if [ "$1" = "run" ]
then
    build && ./$OUT
elif [ "$1" = "release" ]
then
    CFLAGS="$GFLAGS $RFLAGS"
    build;
else
    build;
fi
