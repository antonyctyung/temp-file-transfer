#!/bin/sh -x

ls -hl ./clang/bin/rc6_test
uname -a
lscpu
lsblk
cat /proc/meminfo
free -h

# time python2 ./py/rc6.py
# time ./clang/bin/rc6_test

/usr/bin/time -v ./py/rc6.py
/usr/bin/time -v taskset -c 0 ./clang/bin/rc6_test
/usr/bin/time -v ./clang/bin/rc6_test_pth

perf stat -ddd ./py/rc6.py
perf stat -ddd taskset -c 0 ./clang/bin/rc6_test
perf stat -ddd ./clang/bin/rc6_test_pth
