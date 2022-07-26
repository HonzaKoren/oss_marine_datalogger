#!/usr/bin/env python3
# -*- coding utf-8 -*-

"""
Creates the shared memory segments upon start
"""

from multiprocessing import shared_memory

from common import constants as c

temperature = shared_memory.SharedMemory(name='temperature', create=True, size=256*1024)
temperature = shared_memory.SharedMemory(name='humidity', create=True, size=256*1024)
temperature = shared_memory.SharedMemory(name='location', create=True, size=256*1024)
temperature = shared_memory.SharedMemory(name='pressure', create=True, size=256*1024)
temperature = shared_memory.SharedMemory(name='temperature_2', create=True, size=256*1024)
