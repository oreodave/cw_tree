#!/usr/bin/env sh

set -xe

OUT="cw_tree.out"
GFLAGS="-Wall -Wextra -Wswitch-enum -std=c++17 -Iraylib-5.5_linux_amd64/include"
LIBS="-Lraylib-5.5_linux_amd64/lib -l:libraylib.a" # link statically with raylib
VARFLAGS="-DTHREAD_PAUSE_MS=1000 -DTHREAD_GENERAL_MS=1"

DFLAGS="-ggdb -fsanitize=address -fsanitize=undefined"
RFLAGS="-O2"
CFLAGS="$GFLAGS $VARFLAGS $DFLAGS"
if [ "$1" = "release" ]
then
    CFLAGS="$GFLAGS $VARFLAGS $RFLAGS"
    shift 1
elif [ "$1" = "debug" ]
then
    shift 1
fi

c++ $CFLAGS -o $OUT src/node.cpp src/state.cpp src/worker.cpp src/main.cpp $LIBS
if [ "$1" = "run" ]
then
    ./$OUT
fi
