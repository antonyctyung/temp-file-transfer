#!/usr/bin/env python3

import os



for filename in os.listdir('.'):
    f = os.path.join('.', filename)
    if os.path.isfile(f):
        if (filename.find('.txt') > -1):
            name = os.path.splitext(filename)[0]
            split()
                