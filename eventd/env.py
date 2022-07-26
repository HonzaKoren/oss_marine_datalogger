#!/usr/bin/env python3
import io
import os
from multiprocessing import shared_memory
from pathlib import Path
from shutil import move
import time

import grovepi

import pynmea2
import serial


dht_sensor = 2

tmp_dir = "/home/pi/runtime/log"
tmp_log = os.path.join(tmp_dir, "env.log")
dest_log = "/home/pi/runtime/log/env.{}.log"
persistent = "/home/pi/data/"
persistent_write_offset = 60 # How long before copy log to persistent storage

last = 0

def init():
    # Check dirs
    for dir in (tmp_dir, persistent):
        if not os.path.isdir(dir):
            os.mkdir(dir)

    # Set up sensor for input
    grovepi.pinMode(dht_sensor, 'INPUT')

    # Set up shared memory segments
    temperature = shared_memory.SharedMemory(create=True, size=10)

init()

while 1:
    now = int(time.time())
    try:
        temperature, humidity = grovepi.dht(dht_sensor, 0)
    except Exception as e:
        print('Generic error: {}'.format(e))
        continue
    # Write TMP file
    f = open(tmp_log, "a")
    f.write(f'{time.time()} T={temperature} H={humidity}' + "\n")
    f.close()

    time.sleep(1)

    # If history was not kept for offset seconds, move it to persitent storage
    if now - last > persistent_write_offset:
        dst = dest_log.format(int(time.time()))
        Path(tmp_log).rename(dst)
        move(dst, persistent)
        last = int(time.time())


