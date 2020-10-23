#!/bin/bash

# Lint
clang-format -i main.c

# Cleanup
rm -rf ../site
mkdir ../site

cc -std=c99 -DDEBUG -Wall -Wpedantic -Wshadow -Wextra -Werror=implicit-int -Werror=incompatible-pointer-types -Werror=int-conversion -Wvla -g -Og -fsanitize=address -fsanitize=undefined main.c -o main

./main

rm ./main
