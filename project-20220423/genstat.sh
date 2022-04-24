#!/usr/bin/env bash

# root check
if [[ $(id -u) -ne 0 ]] ; then echo "Please run as root" ; exit 1 ; fi

# print command executed
set -x

# constants
BMARCH=$(uname -m)
BMCORE=$(grep -c ^processor /proc/cpuinfo)
BMSUFFIX="_${BMARCH}_${BMCORE}"
BMPERFCMD="perf stat -e task-clock:u,context-switches:u,cpu-migrations:u,page-faults:u,cycles:u,instructions:u"
BMTSKSCMD="taskset -c 0"
BMPYLOC="./py/rc6.py"
BMCLOC="./clang/bin/rc6_test"
BMPTHLOC="./clang/bin/rc6_test_pth"

# make binaries
cd clang
mkdir bin
mkdir obj
make clean
make
cd ..

# get spec, run perf and store them to file
mkdir stat${BMSUFFIX}
./spec.sh > ./stat${BMSUFFIX}/spec${BMSUFFIX}.txt 2>&1
eval "${BMPERFCMD} ${BMPYLOC}" > ./stat${BMSUFFIX}/stat_py${BMSUFFIX}.txt 2>&1
eval "${BMPERFCMD} ${BMTSKSCMD} ${BMPYLOC}" > ./stat${BMSUFFIX}/stat_py_tsks${BMSUFFIX}.txt 2>&1
eval "${BMPERFCMD} ${BMCLOC}" > ./stat${BMSUFFIX}/stat_c${BMSUFFIX}.txt 2>&1
eval "${BMPERFCMD} ${BMTSKSCMD} ${BMCLOC}" > ./stat${BMSUFFIX}/stat_c_tsks${BMSUFFIX}.txt 2>&1
eval "${BMPERFCMD} ${BMPTHLOC}" > ./stat${BMSUFFIX}/stat_pth${BMSUFFIX}.txt 2>&1
eval "${BMPERFCMD} ${BMTSKSCMD} ${BMPTHLOC}" > ./stat${BMSUFFIX}/stat_pth_tsks${BMSUFFIX}.txt 2>&1

# cleanup
cd clang
make clean
cd ..
