#!/usr/bin/env bash

# root check
if [[ $(id -u) -ne 0 ]] ; then echo "Please run as root" ; exit 1 ; fi

# print command executed
set -x

# constants
BMARCH=$(uname -m)
BMCORE=$(grep -c ^processor /proc/cpuinfo)
BMSUFFIX="-${BMARCH}b${BMCORE}"
BMPERFCMD="perf stat -e task-clock:u,context-switches:u,cpu-migrations:u,page-faults:u,cycles:u,instructions:u"
BMTSKSCMD="taskset -c 0"
BMPYLOC="./py/rc6.py"
BMCBINLOC="./clang/bin/*"
# BMCLOC="./clang/bin/rc6_test"
# BMPTHLOC="./clang/bin/rc6_test_pth"
BML2TESTLOC="/sys/devices/system/cpu/cpu0/cache/index2/level"

# test if l2 cache exists to differentiate riscv64b2l2 and riscv64b4l2w
if [ -f "${BML2TESTLOC}" ]; then
    if [ "${BMARCH}" == "riscv64" ]; then
        BMSUFFIX+="l2"
        if [ "${BMCORE}" == "4" ]; then
            BMSUFFIX+="w"
        fi
    fi
fi

# make binaries
cd clang
mkdir bin
mkdir obj
make clean
make
cd ..

# get spec, run perf and store them to file
mkdir stat
# mkdir stat/stat${BMSUFFIX}
# ./spec.sh > ./stat/stat${BMSUFFIX}/spec${BMSUFFIX}.txt 2>&1
./spec.sh > ./stat/spec${BMSUFFIX}.txt 2>&1

eval "${BMPERFCMD} ${BMPYLOC}" > ./stat/stat-rc6_py${BMSUFFIX}.txt 2>&1
eval "${BMPERFCMD} ${BMTSKSCMD} ${BMPYLOC}" > ./stat/stat-rc6_py_tsks${BMSUFFIX}.txt 2>&1

for f in ${BMCBINLOC}
do
    eval "${BMPERFCMD} ${f}" > ./stat/stat-${f##*/}${BMSUFFIX}.txt 2>&1
    eval "${BMPERFCMD} ${BMTSKSCMD} ${f}" > ./stat/stat-${f##*/}_tsks${BMSUFFIX}.txt 2>&1
done

# eval "${BMPERFCMD} ${BMCLOC}" > ./stat/stat${BMSUFFIX}/stat_c${BMSUFFIX}.txt 2>&1
# eval "${BMPERFCMD} ${BMTSKSCMD} ${BMCLOC}" > ./stat/stat${BMSUFFIX}/stat_c-tsks${BMSUFFIX}.txt 2>&1
# eval "${BMPERFCMD} ${BMPTHLOC}" > ./stat/stat${BMSUFFIX}/stat_pth${BMSUFFIX}.txt 2>&1
# eval "${BMPERFCMD} ${BMTSKSCMD} ${BMPTHLOC}" > ./stat/stat${BMSUFFIX}/stat_pth_tsks${BMSUFFIX}.txt 2>&1

# cleanup
cd clang
make clean
cd ..
