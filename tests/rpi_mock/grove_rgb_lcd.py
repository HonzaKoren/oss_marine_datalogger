"""
Mock library enabling passing script run
"""


def set_text(msg: str):
    """
        Dummy function
    """
    print(f'Got LCD text\n{"="*16}\n{msg}\n{"="*16}\n')


def set_rgb(red: int, green: int, blue: int):
    """
        Dummy function
    """
    print(f'Got LCD color ({red}, {green}, {blue})\n')
