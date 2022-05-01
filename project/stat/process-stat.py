#!/usr/bin/env python3

import os
import sqlite3 as sql
import subprocess as sp
from typing import OrderedDict

statsname = OrderedDict([
    ('bm' , "Benchmark Test"),
    ('soc' , "System"),
    ('task-clock-ms' , "Task Clock (ms)"),
    ('cpu-utilized' , "CPU Utilized"),
    ('context-switches' , "Context Switches"),
    ('context-sw-rate' , "Context Switches Rate"),
    ('cpu-migrations' , "CPU Migration"),
    ('cpu-mgrt-rate' , "CPU Migration Rate"),
    ('page-faults' , "Page Fault"),
    ('page-faults-rate' , "Page Fault Rate"),
    ('cycles' , "CPU Cycle Counts"),
    ('frequency' , "CPU Frequency"),
    ('instructions' , "Instruction Counts"),
    ('ipc' , "Instruction per Cycle"),
    ('time-elapsed' , "Wall Clock Time"),
    ('time-user' , "User Space Time"),
    ('time-sys' , "Kernel Space Time"),
    ('watt' , "Average Power Consumption (mW)")
])

# Overwrite, create and connect db file
if os.path.exists('stat.db'):
    os.remove('stat.db')
con = sql.connect('stat.db')

# import watt.csv
result = sp.run([   'sqlite3',
                    'stat.db',
                    '-cmd',
                    'create table watt(timestamp INT, milivolt INT, miliamp INT, miliwatt REAL)',
                    '.mode csv',
                    '.separator \",\"',
                    '.import -skip 1 watt.csv watt'],
                capture_output=True)

# create master table


for filename in sorted(os.listdir()):
    if os.path.isfile(filename):
        if (filename.find('.txt') > -1):
            if (filename.find('stat') == 0):
                name = os.path.splitext(filename)[0]
                (stat, bm, soc) = name.split('-')
                statentry = {
                    'bm': bm,
                    'soc': soc
                }
                fd = open(filename, 'r')
                lines = fd.readlines()
                for line in lines:
                    stripped = line.strip()
                    if (stripped.find('++ ') == 0):
                        continue
                    elif (stripped.find('task-clock') > -1):
                        (first, second) = stripped.split('#')
                        statentry['task-clock-ms'] = first.split()[0]
                        statentry['cpu-utilized'] = second.split()[0]
                    elif (stripped.find('context-switches') > -1):
                        (first, second) = stripped.split('#')
                        statentry['context-switches'] = first.split()[0]
                        statentry['context-sw-rate'] = second.strip()
                    elif (stripped.find('cpu-migrations') > -1):
                        (first, second) = stripped.split('#')
                        statentry['cpu-migrations'] = first.split()[0]
                        statentry['cpu-mgrt-rate'] = second.strip()
                    elif (stripped.find('page-faults') > -1):
                        (first, second) = stripped.split('#')
                        statentry['page-faults'] = first.split()[0]
                        statentry['page-faults-rate'] = second.strip()
                    elif (stripped.find('cycles') > -1):
                        (first, second) = stripped.split('#')
                        statentry['cycles'] = first.split()[0]
                        statentry['frequency'] = second.strip()
                    elif (stripped.find('instructions') > -1):
                        (first, second) = stripped.split('#')
                        statentry['instructions'] = first.split()[0]
                        statentry['ipc'] = second.split()[0]
                    elif (stripped.find('time elapsed') > -1):
                        statentry['time-elapsed'] = stripped.split()[0]
                    elif (stripped.find('seconds user') > -1):
                        statentry['time-user'] = stripped.split()[0]
                    elif (stripped.find('seconds sys') > -1):
                        statentry['time-sys'] = stripped.split()[0]
                loopbm = bm
                if (bm.find('py') == -1):
                    if (bm.find('_tsks') > -1):
                        loopbm = loopbm.replace('_tsks', '_loop_tsks')
                    else:
                        loopbm += '_loop'
                wattloc = "watt-" + loopbm + "-" + soc + ".txt"
                if os.path.exists(wattloc):
                    wattfd = open(wattloc, 'r')
                    starttime = int(wattfd.readline())
                    endtime = int(wattfd.readline())
                    cur = con.cursor()
                    query = "SELECT miliwatt FROM watt WHERE timestamp BETWEEN " + str(starttime) + " AND " + str(endtime)
                    cur.execute(query)
                    sum = 0
                    count = 0
                    for tpl in cur.fetchall():
                        sum += tpl[0]
                        count += 1
                    statentry['watt'] = sum/count if count else 0
                else:
                    statentry['watt'] = 0
                cur.close()
                print(statentry)
                print("\n")
