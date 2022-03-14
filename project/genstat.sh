#!/bin/sh
cd clang
make
cd ..
./stat_script.sh > stat.txt 2>&1
cd clang
make clean
cd ..
