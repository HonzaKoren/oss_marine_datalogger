# -*- coding: utf-8 -*-

"""
    Mock grovepi module
"""


def digital_read(pin_id):
    """
        Mock digitalRead
    """
    print(f'digitalRead: Got {pin_id}')
    return 1


def pin_mode(pin, mode):
    """
        Mock pinMode
    """
    print(f'pinMode: Got mode {mode} for pin {pin}')
