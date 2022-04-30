#!/usr/bin/env bash
set -x

ls -hl ./clang/bin
ls -hl ./lwc_c/bin
ls -hl ./py/rc6.py

uname -a

lsb_release -a

cat /proc/version

lscpu

cat /proc/cpuinfo

lsmem

cat /proc/meminfo

lsblk

free -h
