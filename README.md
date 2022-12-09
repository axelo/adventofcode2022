# Advent of Code 2022

## C

You need [clang](https://clang.llvm.org/) installed, then run

    clang -fsanitize=address -Wall -Wextra -Wpedantic -Werror -std=c17 -o NNab NNab.c && NNab < input/NN.txt

## Go

You need [go](https://go.dev) installed, then run

    go run NNab.go < input/NN.txt

## Zig

You need [zig](https://ziglang.org) installed, then run

    zig run NNab.zig < input/NN.txt
