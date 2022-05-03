#!/usr/bin/env python3

import os
import sqlite3 as sql
import subprocess as sp
from typing import OrderedDict

statsname = OrderedDict([
    ('bm' , "Benchmark Test"),
    ('soc' , "System"),
    ('task_clock_ms' , "Task Clock (ms)"),
    ('cpu_utilized' , "CPU Utilized"),
    ('context_switches' , "Context Switches"),
    ('context_sw_rate' , "Context Switches per Second"),
    ('cpu_migrations' , "CPU Migration"),
    ('cpu_mgrt_rate' , "CPU Migration Rate per Second"),
    ('page_faults' , "Page Fault"),
    ('page_faults_rate' , "Page Fault Rate per Second"),
    ('cycles' , "CPU Cycle Counts"),
    ('frequency' , "CPU Frequency (GHz)"),
    ('instructions' , "Instruction Counts"),
    ('ipc' , "Instructions per Cycle"),
    ('cpi' , "Cycles per Instruction"),
    ('time_elapsed' , "Wall Clock Time"),
    ('time_user' , "User Space Time"),
    ('time_sys' , "Kernel Space Time"),
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
query = 'CREATE TABLE master('
for key, value in statsname.items():
    query += key
    query += ' TEXT, '
query = query[:-2]
query += ')'
cur = con.cursor()
cur.execute('DROP TABLE IF EXISTS master')
cur.execute(query)
con.commit()
cur.close()

bmset = set()

# enter entries to master table
for filename in sorted(os.listdir()):
    if os.path.isfile(filename):
        if (filename.find('.txt') > -1):
            if (filename.find('stat') == 0):
                name = os.path.splitext(filename)[0]
                (stat, bm, soc) = name.split('-')
                bmset.add(bm)
                statentry = {
                    'bm': bm,
                    'soc': soc
                }
                fd = open(filename, 'r')
                lines = fd.readlines()
                for line in lines:
                    stripped = line.strip().replace(',','')
                    if (stripped.find('++ ') == 0):
                        continue
                    elif (stripped.find('task-clock') > -1):
                        (first, second) = stripped.split('#')
                        statentry['task_clock_ms'] = first.split()[0]
                        statentry['cpu_utilized'] = second.split()[0]
                    elif (stripped.find('context-switches') > -1):
                        (first, second) = stripped.split('#')
                        statentry['context_switches'] = first.split()[0]
                        rate, unit = second.split()
                        rate = float(rate)
                        if (unit.find('K') > -1):
                            rate *= 1000
                        elif (unit.find('M') > -1):
                            rate *= 1000000
                        statentry['context_sw_rate'] = str(rate)
                    elif (stripped.find('cpu-migrations') > -1):
                        (first, second) = stripped.split('#')
                        statentry['cpu_migrations'] = first.split()[0]
                        rate, unit = second.split()
                        rate = float(rate)
                        if (unit.find('K') > -1):
                            rate *= 1000
                        elif (unit.find('M') > -1):
                            rate *= 1000000
                        statentry['cpu_mgrt_rate'] = str(rate)
                    elif (stripped.find('page-faults') > -1):
                        (first, second) = stripped.split('#')
                        statentry['page_faults'] = first.split()[0]
                        rate, unit = second.split()
                        rate = float(rate)
                        if (unit.find('K') > -1):
                            rate *= 1000
                        elif (unit.find('M') > -1):
                            rate *= 1000000
                        statentry['page_faults_rate'] = str(rate)
                    elif (stripped.find('cycles') > -1):
                        (first, second) = stripped.split('#')
                        statentry['cycles'] = first.split()[0]
                        statentry['frequency'] = second.split()[0]
                    elif (stripped.find('instructions') > -1):
                        (first, second) = stripped.split('#')
                        statentry['instructions'] = first.split()[0]
                        statentry['ipc'] = second.split()[0]
                    elif (stripped.find('time elapsed') > -1):
                        statentry['time_elapsed'] = stripped.split()[0]
                    elif (stripped.find('seconds user') > -1):
                        statentry['time_user'] = stripped.split()[0]
                    elif (stripped.find('seconds sys') > -1):
                        statentry['time_sys'] = stripped.split()[0]
                statentry['cpi'] = str(int(statentry['cycles'])/int(statentry['instructions'])) if int(statentry['instructions']) else '0'
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
                    cur.close()
                else:
                    statentry['watt'] = 0
                cur = con.cursor()
                query = "INSERT INTO master VALUES (\'"
                for key, value in statsname.items():
                    query += str(statentry[key])
                    query += '\', \''
                query = query[:-3]
                query += ')'
                cur = con.cursor()
                # print(query)
                cur.execute(query)
                con.commit()
                cur.close()

# Generate .csv for each benchmark 
for bm in bmset:
    query = "SELECT * FROM master WHERE bm = \"" + bm + "\" ORDER BY soc"
    cur = con.cursor()
    cur.execute(query)
    ziplist = list(zip(*(cur.fetchall())))
    cur.close()
    csvname = bm + ".csv"
    if os.path.exists(csvname):
        os.remove(csvname)
    fd = open(csvname, 'w')
    for i in range(1,len(ziplist)):
        (key, desc) = list(statsname.items())[i]
        fd.write(desc+',')
        for j in ziplist[i]:
            if (i > 1):
                wrstr = f'{float(j):.3f},'
            else:
                wrstr = j + ','
            fd.write(wrstr.replace('.000',''))
        fd.write('\n')
    fd.close()

# Close DB connection
con.close()