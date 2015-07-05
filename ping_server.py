#!/usr/bin/env python3
# coding: utf-8

import socketserver
import sys


class UDPHandler(socketserver.BaseRequestHandler):
    """Simple PONG server."""

    def handle(self):
        data = self.request[0]
        socket = self.request[1]
        print("{} PING'd: {}".format(self.client_address[0], data))
        socket.sendto(data, self.client_address)

if __name__ == "__main__":
    try:
        HOST = str(sys.argv[1])
    except (IndexError, ValueError):
        HOST = "localhost"

    try:
        PORT = int(sys.argv[2])
    except (IndexError, ValueError):
        PORT = 9876

    server = socketserver.UDPServer((HOST, PORT), UDPHandler)
    server.serve_forever()
