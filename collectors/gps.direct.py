#!/usr/bin/env python3
import io

import pynmea2
import serial


ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1.0)
sio = io.TextIOWrapper(io.BufferedRWPair(ser, ser))

while 1:
    try:
        line = sio.readline()
        msg = pynmea2.parse(line)
    except UnicodeDecodeError:
        continue
    except serial.SerialException as e:
        # print('Device error: {}'.format(e))
        break
    except pynmea2.ParseError as e:
        # print('Parse error: {}'.format(e))
        continue
    f = open("../gps.data.log", "a")
    f.write(line)
    f.close()
