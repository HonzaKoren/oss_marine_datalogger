# -*- coding: utf8 -*-
"""
    Mock pigpio lib
"""


Error = Exception


class Pi:
    """
        Dummy pi class to moc RPi pigpio implementation
    """

    def i2c_open(self, bus: int, device: int) -> str:
        """
            Mock
        """
        print(f'{self.__class__.__name__}: Bus {bus}, device {device}')
        return 'OK'

    def i2c_close(self, desc: str) -> bool:
        """
            Mock
        """
        print(f'{self.__class__.__name__}: Desc {desc}')
        return True

    def i2c_read_byte(self, desc: str):
        """
            Mock
        """
        print(f'{self.__class__.__name__}: Desc {desc}')
        return 'x'

    def stop(self):
        """
            Mock
        """
