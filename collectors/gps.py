#!/usr/bin/env python3
import io
import os
from pathlib import Path
from shutil import move
import time

import pynmea2
import serial


ser = serial.Serial('/dev/serial0', 9600, timeout=1.0)
sio = io.TextIOWrapper(io.BufferedRWPair(ser, ser))

tmp_dir = "/home/pi/runtime/log"
tmp_log = os.path.join(tmp_dir, "gps.log")
dest_log = "/home/pi/runtime/log/gps.{}.log"
persistent = "/home/pi/data/"
persistent_write_offset = 60 # How long before copy log to persistent storage

last = 0

def init():
    # Check dirs
    for dir in (tmp_dir, persistent):
        if not os.path.isdir(dir):
            os.mkdir(dir)

init()

while 1:
    now = int(time.time())
    try:
        line = sio.readline()
        msg = pynmea2.parse(line)
    except UnicodeDecodeError:
        continue
    except serial.SerialException as e:
        print('Device error: {}'.format(e))
        break
    except pynmea2.ParseError as e:
        print('Parse error: {}'.format(e))
        continue
    # Write TMP file
    f = open(tmp_log, "a")
    f.write(repr(msg) + "\n")
    f.close()

    # If history was not kept for offset seconds, move it to persitent storage
    if now - last > persistent_write_offset:
        dst = dest_log.format(int(time.time()))
        Path(tmp_log).rename(dst)
        move(dst, persistent)
        last = int(time.time())


