#!/usr/bin/env python3
# coding: utf-8

"""
Algorithm scaffolding (based on a state machine).
"""

import asyncio
from functools import partial


def turn_off():
    """Cut off the power from the batteries to the engine."""
    pass


def turn_on():
    """Bring back the power to the engine."""
    pass


if __name__ == '__main__':
    state = START

    try:
        while True:
            if state == START:
                pass
            elif state == CONNECTION_WAIT:
                pass
            elif state == CONNECTED:
                pass
            elif state == DIAGNOSTIC_DATA:
                pass
            elif state == PING:
                pass
            elif state == PONG_WAIT:
                pass
            elif state == STOP:
                pass
            elif state == RESET:
                pass
            else:
                # error?
                pass
    except KeyboardInterrupt:
        print('Transmisja przerwana!')
    finally:
        pass
