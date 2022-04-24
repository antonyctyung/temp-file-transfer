#!/usr/bin/env bash
set -x

ls -hl ./clang/bin

uname -a

cat /proc/version

lscpu

cat /proc/cpuinfo

lsblk

lsmem

cat /proc/meminfo

free -h
