#!/usr/bin/env bash

# root check
if [[ $(id -u) -ne 0 ]] ; then echo "Please run as root" ; exit 1 ; fi

# args check
if [ "$1" == "spec" ] ; then
    :
elif [ "$1" == "watt" ] ; then
    :
elif [ "$1" == "c" ] ; then
    :
elif [ "$1" == "rc6_c" ] ; then
    :
elif [ "$1" == "lwc_c" ] ; then
    :
elif [ "$1" == "py" ] ; then
    :
elif [ "$1" == "all" ] ; then
    :
elif [ "$1" == "all_but_watt" ] ; then
    :
else
    echo "run with one of the arguments: spec, watt, c, rc6_c, lwc_c, py, all_but_watt or all"
    exit 1
fi

# print command executed
set -x

# constants
BMARCH=$(uname -m)
BMCORE=$(grep -c ^processor /proc/cpuinfo)
BMSUFFIX="${BMARCH}b${BMCORE}"
BMPERFCMD="perf stat -e task-clock:u,context-switches:u,cpu-migrations:u,page-faults:u,cycles:u,instructions:u"
BMTSKSCMD="taskset -c 0"
BMTMOTCMD="timeout --foreground 60s"
BMEPCHCMD="date +%s"
BMPYLOC="./py/rc6.py"
BMPYLOOPLOC="./py/rc6.py"
BMCBINLOC="./clang/bin/*"
BMLWCCBINLOC="./lwc_c/bin/*"
# BMCLOC="./clang/bin/rc6_test"
# BMPTHLOC="./clang/bin/rc6_test_pth"
BML2TESTLOC="/sys/devices/system/cpu/cpu0/cache/index2/level"

# test if l2 cache exists to differentiate riscv64b2l2 and riscv64b4l2w
if [ -f "${BML2TESTLOC}" ] ; then
    if [ "${BMARCH}" == "riscv64" ] ; then
        BMSUFFIX+="l2"
        if [ "${BMCORE}" == "4" ] ; then
            BMSUFFIX+="w"
        fi
    fi
fi

f_spec () {
    echo "---------- Generating Specification\n"
    cd clang ; make clean ; make all ; cd ..
    cd lwc_c ; make clean ; make all ; cd ..
    ./spec.sh > ./stat/spec-${BMSUFFIX}.txt 2>&1
}

f_stat_lwc_c () {
    echo "---------- Generating LWC Performance Statistics\n"
    cd lwc_c ; make clean ; make all ; cd ..
    for f in ${BMLWCCBINLOC} ; do
        eval "${BMPERFCMD} ${f}" > ./stat/stat-${f##*/}-${BMSUFFIX}.txt 2>&1
        eval "${BMPERFCMD} ${BMTSKSCMD} ${f}" > ./stat/stat-${f##*/}_tsks-${BMSUFFIX}.txt 2>&1
    done
}

f_stat_rc6_c () {
    echo "---------- Generating RC6 Performance Statistics\n"
    cd clang ; make clean ; make all ; cd ..
    for f in ${BMCBINLOC} ; do
        eval "${BMPERFCMD} ${f}" > ./stat/stat-${f##*/}-${BMSUFFIX}.txt 2>&1
        eval "${BMPERFCMD} ${BMTSKSCMD} ${f}" > ./stat/stat-${f##*/}_tsks-${BMSUFFIX}.txt 2>&1
    done
}

f_stat_py () {
    echo "---------- Generating Python RC6 Performance Statistics\n"
    eval "${BMPERFCMD} ${BMPYLOC}" > ./stat/stat-rc6_py-${BMSUFFIX}.txt 2>&1
    eval "${BMPERFCMD} ${BMTSKSCMD} ${BMPYLOC}" > ./stat/stat-rc6_py_tsks-${BMSUFFIX}.txt 2>&1
}

f_watt () {
    echo "---------- Running Each Binaruy for One Minute\n"
    cd clang ; make clean ; make all_loop ; cd ..
    cd lwc_c ; make clean ; make all_loop ; cd ..
    eval "${BMEPCHCMD}" > ./stat/watt-idle-${BMSUFFIX}.txt
    sleep 60s
    echo "\n" >> ./stat/watt-idle-${BMSUFFIX}.txt
    eval "${BMEPCHCMD}" >> ./stat/watt-idle-${BMSUFFIX}.txt
    eval "${BMEPCHCMD}" > ./stat/watt-rc6_py-${BMSUFFIX}.txt
    eval "${BMTMOTCMD} ${BMPYLOOPLOC}"
    echo "\n" >> ./stat/watt-rc6_py-${BMSUFFIX}.txt
    eval "${BMEPCHCMD}" >> ./stat/watt-rc6_py-${BMSUFFIX}.txt
    eval "${BMEPCHCMD}" > ./stat/watt-rc6_py_tsks-${BMSUFFIX}.txt
    eval "${BMTMOTCMD} ${BMTSKSCMD} ${BMPYLOOPLOC}"
    echo " " >> ./stat/watt-rc6_py_tsks-${BMSUFFIX}.txt
    eval "${BMEPCHCMD}" >> ./stat/watt-rc6_py_tsks-${BMSUFFIX}.txt
    for f in ${BMCBINLOC} ; do
        eval "${BMEPCHCMD}" > ./stat/watt-${f##*/}-${BMSUFFIX}.txt
        eval "${BMTMOTCMD} ${f}"
        echo "\n" >> ./stat/watt-${f##*/}-${BMSUFFIX}.txt
        eval "${BMEPCHCMD}" >> ./stat/watt-${f##*/}-${BMSUFFIX}.txt
        eval "${BMEPCHCMD}" > ./stat/watt-${f##*/}_tsks-${BMSUFFIX}.txt
        eval "${BMTMOTCMD} ${BMTSKSCMD} ${f}"
        echo "\n" >> ./stat/watt-${f##*/}-${BMSUFFIX}.txt
        eval "${BMEPCHCMD}" >> ./stat/watt-${f##*/}_tsks-${BMSUFFIX}.txt
    done
    for f in ${BMLWCCBINLOC} ; do
        eval "${BMEPCHCMD}" > ./stat/watt-${f##*/}-${BMSUFFIX}.txt
        eval "${BMTMOTCMD} ${f}"
        echo "\n" >> ./stat/watt-${f##*/}-${BMSUFFIX}.txt
        eval "${BMEPCHCMD}" >> ./stat/watt-${f##*/}-${BMSUFFIX}.txt
        eval "${BMEPCHCMD}" > ./stat/watt-${f##*/}_tsks-${BMSUFFIX}.txt
        eval "${BMTMOTCMD} ${BMTSKSCMD} ${f}"
        echo "\n" >> ./stat/watt-${f##*/}-${BMSUFFIX}.txt
        eval "${BMEPCHCMD}" >> ./stat/watt-${f##*/}_tsks-${BMSUFFIX}.txt
    done
}


# get spec and store them to file
mkdir stat
if [ "$1" == "spec" ]; then
    # cd clang ; make clean ; make all ; cd ..
    # cd lwc_c ; make clean ; make all ; cd ..
    # ./spec.sh > ./stat/spec-${BMSUFFIX}.txt 2>&1
    f_spec
elif [ "$1" == "c" ]; then
    # cd clang ; make clean ; make all ; cd ..
    # cd lwc_c ; make clean ; make all ; cd ..
    # for f in ${BMCBINLOC} ; do
    #     eval "${BMPERFCMD} ${f}" > ./stat/stat-${f##*/}-${BMSUFFIX}.txt 2>&1
    #     eval "${BMPERFCMD} ${BMTSKSCMD} ${f}" > ./stat/stat-${f##*/}_tsks-${BMSUFFIX}.txt 2>&1
    # done
    # for f in ${BMLWCCBINLOC} ; do
    #     eval "${BMPERFCMD} ${f}" > ./stat/stat-${f##*/}-${BMSUFFIX}.txt 2>&1
    #     eval "${BMPERFCMD} ${BMTSKSCMD} ${f}" > ./stat/stat-${f##*/}_tsks-${BMSUFFIX}.txt 2>&1
    # done
    f_stat_rc6_c
    f_stat_lwc_c
elif [ "$1" == "rc6_c" ]; then
    # cd clang ; make clean ; make all ; cd ..
    # for f in ${BMCBINLOC} ; do
    #     eval "${BMPERFCMD} ${f}" > ./stat/stat-${f##*/}-${BMSUFFIX}.txt 2>&1
    #     eval "${BMPERFCMD} ${BMTSKSCMD} ${f}" > ./stat/stat-${f##*/}_tsks-${BMSUFFIX}.txt 2>&1
    # done
    f_stat_rc6_c
elif [ "$1" == "lwc_c" ]; then
    # cd lwc_c ; make clean ; make all ; cd ..
    # for f in ${BMLWCCBINLOC} ; do
    #     eval "${BMPERFCMD} ${f}" > ./stat/stat-${f##*/}-${BMSUFFIX}.txt 2>&1
    #     eval "${BMPERFCMD} ${BMTSKSCMD} ${f}" > ./stat/stat-${f##*/}_tsks-${BMSUFFIX}.txt 2>&1
    # done
    f_stat_lwc_c
elif [ "$1" == "py" ] ; then
    # eval "${BMPERFCMD} ${BMPYLOC}" > ./stat/stat-rc6_py-${BMSUFFIX}.txt 2>&1
    # eval "${BMPERFCMD} ${BMTSKSCMD} ${BMPYLOC}" > ./stat/stat-rc6_py_tsks-${BMSUFFIX}.txt 2>&1
    f_stat_py
elif [ "$1" == "all" ] ; then
    # cd clang ; make clean ; make all ; cd ..
    # cd lwc_c ; make clean ; make all ; cd ..
    # ./spec.sh > ./stat/spec-${BMSUFFIX}.txt 2>&1
    # eval "${BMPERFCMD} ${BMPYLOC}" > ./stat/stat-rc6_py-${BMSUFFIX}.txt 2>&1
    # eval "${BMPERFCMD} ${BMTSKSCMD} ${BMPYLOC}" > ./stat/stat-rc6_py_tsks-${BMSUFFIX}.txt 2>&1
    #     for f in ${BMCBINLOC} ; do
    #     eval "${BMPERFCMD} ${f}" > ./stat/stat-${f##*/}-${BMSUFFIX}.txt 2>&1
    #     eval "${BMPERFCMD} ${BMTSKSCMD} ${f}" > ./stat/stat-${f##*/}_tsks-${BMSUFFIX}.txt 2>&1
    # done
    # for f in ${BMLWCCBINLOC} ; do
    #     eval "${BMPERFCMD} ${f}" > ./stat/stat-${f##*/}-${BMSUFFIX}.txt 2>&1
    #     eval "${BMPERFCMD} ${BMTSKSCMD} ${f}" > ./stat/stat-${f##*/}_tsks-${BMSUFFIX}.txt 2>&1
    # done
    # cd clang ; make clean ; make all_loop ; cd ..
    # cd lwc_c ; make clean ; make all_loop ; cd ..
    # eval "${BMEPCHCMD}" > ./stat/watt-rc6_py-${BMSUFFIX}.txt
    # eval "${BMTMOTCMD} ${BMPYLOC}"
    # echo " " >> ./stat/watt-rc6_py-${BMSUFFIX}.txt
    # eval "${BMEPCHCMD}" >> ./stat/watt-rc6_py-${BMSUFFIX}.txt
    # eval "${BMEPCHCMD}\n" > ./stat/watt-rc6_py_tsks-${BMSUFFIX}.txt
    # eval "${BMTMOTCMD} ${BMTSKSCMD} ${BMPYLOC}"
    # echo " " >> ./stat/watt-rc6_py_tsks-${BMSUFFIX}.txt
    # echo "${BMEPCHCMD}" >> ./stat/watt-rc6_py_tsks-${BMSUFFIX}.txt
    # for f in ${BMCBINLOC} ; do
    #     eval "${BMEPCHCMD} ${f}" > ./stat/watt-${f##*/}-${BMSUFFIX}.txt
    #     eval "${BMTMOTCMD} ${f}"
    #     echo " " >> ./stat/watt-${f##*/}-${BMSUFFIX}.txt
    #     eval "${BMEPCHCMD} ${f}" >> ./stat/watt-${f##*/}-${BMSUFFIX}.txt
    #     eval "${BMEPCHCMD} ${BMTSKSCMD} ${f}" > ./stat/watt-${f##*/}_tsks-${BMSUFFIX}.txt
    #     eval "${BMTMOTCMD} ${BMTSKSCMD} ${f}"
    #     echo " " >> ./stat/watt-${f##*/}-${BMSUFFIX}.txt
    #     eval "${BMEPCHCMD} ${BMTSKSCMD} ${f}" >> ./stat/watt-${f##*/}_tsks-${BMSUFFIX}.txt
    # done
    # for f in ${BMLWCCBINLOC} ; do
    #     eval "${BMEPCHCMD} ${f}" > ./stat/watt-${f##*/}-${BMSUFFIX}.txt
    #     eval "${BMTMOTCMD} ${f}"
    #     echo " " >> ./stat/watt-${f##*/}-${BMSUFFIX}.txt
    #     eval "${BMEPCHCMD} ${f}" >> ./stat/watt-${f##*/}-${BMSUFFIX}.txt
    #     eval "${BMEPCHCMD} ${BMTSKSCMD} ${f}" > ./stat/watt-${f##*/}_tsks-${BMSUFFIX}.txt
    #     eval "${BMTMOTCMD} ${BMTSKSCMD} ${f}"
    #     echo " " >> ./stat/watt-${f##*/}-${BMSUFFIX}.txt
    #     eval "${BMEPCHCMD} ${BMTSKSCMD} ${f}" >> ./stat/watt-${f##*/}_tsks-${BMSUFFIX}.txt
    # done
    f_spec
    f_stat_py
    f_stat_rc6_c
    f_stat_lwc_c
    f_watt
elif [ "$1" == "watt" ] ; then
    # cd clang ; make clean ; make all_loop ; cd ..
    # cd lwc_c ; make clean ; make all_loop ; cd ..
    # eval "${BMEPCHCMD}" > ./stat/watt-rc6_py-${BMSUFFIX}.txt
    # eval "${BMTMOTCMD} ${BMPYLOC}"
    # echo " " >> ./stat/watt-rc6_py-${BMSUFFIX}.txt
    # eval "${BMEPCHCMD}" >> ./stat/watt-rc6_py-${BMSUFFIX}.txt
    # eval "${BMEPCHCMD}\n" > ./stat/watt-rc6_py_tsks-${BMSUFFIX}.txt
    # eval "${BMTMOTCMD} ${BMTSKSCMD} ${BMPYLOC}"
    # echo " " >> ./stat/watt-rc6_py_tsks-${BMSUFFIX}.txt
    # echo "${BMEPCHCMD}" >> ./stat/watt-rc6_py_tsks-${BMSUFFIX}.txt
    # for f in ${BMCBINLOC} ; do
    #     eval "${BMEPCHCMD} ${f}" > ./stat/watt-${f##*/}-${BMSUFFIX}.txt
    #     eval "${BMTMOTCMD} ${f}"
    #     echo " " >> ./stat/watt-${f##*/}-${BMSUFFIX}.txt
    #     eval "${BMEPCHCMD} ${f}" >> ./stat/watt-${f##*/}-${BMSUFFIX}.txt
    #     eval "${BMEPCHCMD} ${BMTSKSCMD} ${f}" > ./stat/watt-${f##*/}_tsks-${BMSUFFIX}.txt
    #     eval "${BMTMOTCMD} ${BMTSKSCMD} ${f}"
    #     echo " " >> ./stat/watt-${f##*/}-${BMSUFFIX}.txt
    #     eval "${BMEPCHCMD} ${BMTSKSCMD} ${f}" >> ./stat/watt-${f##*/}_tsks-${BMSUFFIX}.txt
    # done
    # for f in ${BMLWCCBINLOC} ; do
    #     eval "${BMEPCHCMD} ${f}" > ./stat/watt-${f##*/}-${BMSUFFIX}.txt
    #     eval "${BMTMOTCMD} ${f}"
    #     echo " " >> ./stat/watt-${f##*/}-${BMSUFFIX}.txt
    #     eval "${BMEPCHCMD} ${f}" >> ./stat/watt-${f##*/}-${BMSUFFIX}.txt
    #     eval "${BMEPCHCMD} ${BMTSKSCMD} ${f}" > ./stat/watt-${f##*/}_tsks-${BMSUFFIX}.txt
    #     eval "${BMTMOTCMD} ${BMTSKSCMD} ${f}"
    #     echo " " >> ./stat/watt-${f##*/}-${BMSUFFIX}.txt
    #     eval "${BMEPCHCMD} ${BMTSKSCMD} ${f}" >> ./stat/watt-${f##*/}_tsks-${BMSUFFIX}.txt
    # done
    f_watt
elif [ "$1" == "all_but_watt" ] ; then
    # cd clang ; make clean ; make all ; cd ..
    # cd lwc_c ; make clean ; make all ; cd ..
    # ./spec.sh > ./stat/spec-${BMSUFFIX}.txt 2>&1
    # eval "${BMPERFCMD} ${BMPYLOC}" > ./stat/stat-rc6_py-${BMSUFFIX}.txt 2>&1
    # eval "${BMPERFCMD} ${BMTSKSCMD} ${BMPYLOC}" > ./stat/stat-rc6_py_tsks-${BMSUFFIX}.txt 2>&1
    # for f in ${BMCBINLOC} ; do
    #     eval "${BMPERFCMD} ${f}" > ./stat/stat-${f##*/}-${BMSUFFIX}.txt 2>&1
    #     eval "${BMPERFCMD} ${BMTSKSCMD} ${f}" > ./stat/stat-${f##*/}_tsks-${BMSUFFIX}.txt 2>&1
    # done
    # for f in ${BMLWCCBINLOC} ; do
    #     eval "${BMPERFCMD} ${f}" > ./stat/stat-${f##*/}-${BMSUFFIX}.txt 2>&1
    #     eval "${BMPERFCMD} ${BMTSKSCMD} ${f}" > ./stat/stat-${f##*/}_tsks-${BMSUFFIX}.txt 2>&1
    # done
    f_spec
    f_stat_py
    f_stat_rc6_c
    f_stat_lwc_c
else
    echo "run with one of the arguments: spec, watt, c, rc6_c, lwc_c, py, all_but_watt or all"
    exit 1
fi

# cleanup
cd clang ; make clean ; cd ..
cd lwc_c ; make clean ; cd ..
