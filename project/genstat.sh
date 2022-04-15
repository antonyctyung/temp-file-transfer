#!/bin/sh
cd clang
mkdir bin
mkdir obj
make
cd ..
./stat_script.sh > stat.txt 2>&1
cd clang
make clean
cd ..
