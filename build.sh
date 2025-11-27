#!/usr/bin/env sh

set -xe

GFLAGS="-Wall -Wextra -Wswitch-enum -std=c++17"
DFLAGS="-ggdb -fsanitize=address -fsanitize=undefined"
CFLAGS="$GFLAGS $DFLAGS"
LIBS="-lraylib -lm"

c++ $CFLAGS -o cw_tree.out src/node.cpp src/state.cpp src/worker.cpp src/numerics.cpp src/main.cpp $LIBS
