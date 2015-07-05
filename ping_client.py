#!/usr/bin/env python3
# coding: utf-8

from itertools import count
import time
import socket
import sys


if __name__ == "__main__":
    try:
        HOST = str(sys.argv[1])
    except (IndexError, ValueError):
        HOST = "localhost"

    try:
        PORT = int(sys.argv[2])
    except (IndexError, ValueError):
        PORT = 9876

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    for i in count(1):
        data = bytes("{}".format(i), encoding="utf-8")
        sock.sendto(data, (HOST, PORT))
        print("Sent PING {}".format(data))
        rcv = sock.recv(1024)
        print("Received PONG {}".format(rcv))
        time.sleep(1)
