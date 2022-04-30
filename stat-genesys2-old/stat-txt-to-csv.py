#!/usr/bin/env python3

import os

statentries = []
statsname = (
    'task-clock-ms',
    'cpu-utilized',
    'context-switches',
    'context-sw-rate',
    'cpu-migrations',
    'cpu-mgrt-rate',
    'page-faults',
    'page-faults-rate',
    'cycles',
    'frequency',
    'instructions',
    'ipc',
    'time-elapsed',
    'time-user',
    'time-sys'
)
csv = []

def makenewcsv(bin):
    

for filename in sorted(os.listdir()):
    if os.path.isfile(filename):
        if (filename.find('.txt') > -1):
            if (filename.find('stat') == 0):
                name = os.path.splitext(filename)[0]
                (stat, binname, soc) = name.split('-')
                statentry = {
                    'bin': binname,
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
                        statentry['constext-switches'] = first.split()[0]
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
                print(statentry)
                statentries.append(statentry)