#!/usr/bin/env python3
# coding: utf-8

import socket
import time

import RPi.GPIO as GPIO

# Globals. We love them, don't we?
STOP_SEND = False

# pin setup
STOP_PIN = 17
RXTX_PIN = 27  # 21 in rev1

# remote setup
HOST = "moduleB"
PORT = 8888


def stop_btn(channel):
    global STOP_SEND
    STOP_SEND = True
    print("[PRESSED] stop")


def ping(stop=False):
    data = ""
    if stop:
        print("[SENDING] stop & ping")
        data = "stop,ping"
    else:
        print("[SENDING] ping")
        data = "ping"

    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((HOST, PORT))
        sock.sendall(bytes(data + "\n", "utf-8"))

        rv = str(sock.recv(1024), "utf-8")

    finally:
        sock.close()

    return rv


def setup():
    GPIO.setmode(GPIO.BCM)  # SoC numbering of pins

    GPIO.setup(STOP_PIN, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
    GPIO.setup(RXTX_PIN, GPIO.OUT)

    GPIO.add_event_detect(STOP_PIN, GPIO.RISING, callback=stop_btn,
                          bouncetime=300)


if __name__ == '__main__':
    setup()

    try:
        while True:
            time.sleep(1)

            GPIO.output(RXTX_PIN, True)
            result = ping(stop=STOP_SEND)
            STOP_SEND = False  # reset anyway
            print("[RESPONSE] {result}".format(result=result))
            GPIO.output(RXTX_PIN, False)

    except KeyboardInterrupt:
        pass
    finally:
        GPIO.cleanup()
