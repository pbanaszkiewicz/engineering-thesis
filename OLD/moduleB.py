#!/usr/bin/env python3
# coding: utf-8

import socketserver
import time

import RPi.GPIO as GPIO

STATUS = True

# pin setup
RESET_PIN = 22
RXTX_PIN = 21  # 27 in rev2
STATUS_PIN = 17

# local setup
HOST = "0.0.0.0"
PORT = 8888


def reset_btn(channel):
    global STATUS
    print("[PRESSED] reset")
    STATUS = True
    GPIO.output(STATUS_PIN, STATUS)


class Server(socketserver.StreamRequestHandler):
    def handle(self):
        global STATUS

        GPIO.output(RXTX_PIN, True)

        self.data = self.rfile.readline().strip()
        if b"stop" in self.data:
            STATUS = False
            GPIO.output(STATUS_PIN, STATUS)

        print("[RECEIVED] \"{}\" from {}".format(self.data,
                                                 self.client_address[0]))
        self.wfile.write(self.data)

        GPIO.output(RXTX_PIN, False)


def setup():
    GPIO.setmode(GPIO.BCM)  # SoC numbering of pins

    GPIO.setup(RESET_PIN, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)

    GPIO.setup(STATUS_PIN, GPIO.OUT, initial=STATUS)
    GPIO.setup(RXTX_PIN, GPIO.OUT)

    GPIO.add_event_detect(RESET_PIN, GPIO.RISING, callback=reset_btn,
                          bouncetime=300)

    server = socketserver.TCPServer((HOST, PORT), Server)
    return server


if __name__ == '__main__':
    server = setup()

    try:
        server.serve_forever()

    except KeyboardInterrupt:
        pass

    finally:
        GPIO.cleanup()
