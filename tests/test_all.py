#!/bin/env python3
"""
    Test for evaluation off correct wiring.
    This test assumes, that wiring was made upon documentation.
    ToDo: Enable some button detection, so test can offer also an initial config generation.
"""

from typing import Tuple, Optional
from time import sleep

try:
    from grove_rgb_lcd import setText, setRGB
except ImportError:
    from rpi_mock.grove_rgb_lcd import set_text as setText, set_rgb as setRGB

# Colors
GREEN_DARK = (0, 50, 0)
GREEN_BRIGHT = (50, 255, 10)
GREEN = (0, 255, 0)


def lcd(msg: str, color: Tuple[int, int, int], timeout: Optional[float] = 0.0):
    """
    Uniform LCD handler to set both, text and LCD color at once
    """
    setText(msg)
    setRGB(*color)
    sleep(timeout)


if __name__ == '__main__':
    # Initial test procedure testing all functionality
    result = {
        'lcd_colors': False,
        'lcd_text': False,
        'button_enter': False,
        'button_up': False,
        'button_down': False,
        'buzzer': False,
        'temperature': False,
        'humidity': False,
        'gps': False,
        'accelerometer': False,
        'barometer': False,
    }

    lcd('Hello captain!', GREEN_DARK)
    sleep(3)
    lcd('The wiring test\nis about to start.', GREEN)
    sleep(3)
    lcd('The wiring test\nis about to start.', GREEN_BRIGHT)
    sleep(3)
