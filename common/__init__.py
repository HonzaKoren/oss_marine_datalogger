# -*- coding utf-8 -*-

from multiprocessing import shared_memory
from dataclasses import dataclass

cfg = {
    'admin_pwd': None,
    'buttons': {
        'enter': 4,
        'down': 7,
        'up': 8,
    },
    'sensors': {
        'gps': '/dev/ttyAMA0',
        'dht11': 2,
        'lcd_text': {'type': 'i2c', 'address': '0x3e'},
        'lcd_rgb': {'type': 'i2c', 'address': '0x62'},
        'accelerometer': {'type': 'i2c', 'address': '0x68'},  # MPU-9250
        'barometer': {'type': 'i2c', 'address': '0x77'},  # BMP180
    }
}

sensors = {
    'temp_dht11': None,
    'humidity_dht11': None,
    'temp_bmp180': None,
    'pressure_bmp180': None,
    'altitude_bmp180': None,
    'admin_pwd': None,
    
}

@dataclass
class Constants:
    """
    Keep all constants in one place
    """
    temperature: 'mdl_temp_1'  # DHT11 sensor
    humidity: 'mdl_hmdt'  # DHT11 sensor
    temperature_1: 'mdl_temp_2'  # BMP180
    air_pressure: 'mdl_air_psr'  # BMP180
    air_altitude: 'mdl_alt'  # BMP180
    bdgsv: 'mdl_bdgsv'  # Air530 bdgsv record buffer
    gngga: 'mdl_gngga'  # Air530 gngga record buffer
    gngll: 'mdl_gngll'  # Air530 gngll record buffer
    gngsa: 'mdl_gngsa'  # Air530 gngsa record buffer
    gnrmc: 'mdl_gnrmc'  # Air530 gnrmc record buffer
    gnvtg: 'mdl_gnvtg'  # Air530 gnvtg record buffer
    gnzda: 'mdl_gnzda'  # Air530 gnzda record buffer
    gpgsv: 'mdl_gpgsv'  # Air530 gpgsv record buffer
    gptxt: 'mdl_gptxt'  # Air530 gptxt record buffer


x = shared_memory.SharedMemory(name='mdl_temp', create=True, size=24)
c = np.ndarray((6,), dtype=np.int64, buffer=x.buf)
len(x.buf)
x.unlink()
x = shared_memory.SharedMemory(name='mdl_temp', create=True, size=64)
len(x.buf)
c = np.ndarray((6,), dtype=np.int64, buffer=x.buf)
c
c[-1] = 8888
c
c = np.ndarray((6,), dtype=np.float64, buffer=x.buf)
c
c[-1] = -230.54874
c
c[-1] = 32.54
c = np.ndarray((1,), dtype=np.float64, buffer=x.buf)
c
c[0]
c[0] = 456
c[0] = 456.45645
c[0] = 456.4564
