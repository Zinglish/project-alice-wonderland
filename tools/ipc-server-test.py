import socket
import sys
import struct
import time

from EventParser import *

# Create a UDS socket
sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

try:
	sock.connect('/tmp/wonderland')
except socket.error, msg:
	print >>sys.stderr, msg
	sys.exit(1)

print("Connection to IPC successful, requesting rabbithole to be made")

payload = "RABBITHOLE"

packet = struct.pack('>I', 1)
packet += struct.pack('>I', len(payload))
packet += payload

sock.send(packet)
print("Sent request")
rx = sock.recv(1024)
print("Received rabbithole: " + rx[4:])
sock.close()

print("Attempting to connect to rabbithole: " + rx[4:])

# Conn to the rabbithole
rabbithole = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

try:
	rabbithole.connect(rx[4:])
except socket.error, msg:
	print >>sys.stderr, msg
	sys.exit(1)

print("Rabbithole connection successful")

while True:
	rx = rabbithole.recv(512)
	
	eventParser = EventParser()
	

	payload = struct.pack('>I', len("BCASTCHAT"))
	payload += "BCASTCHAT"
	payload += "\x01"
	payload += "\x03"
	payload += struct.pack('>I', len("Alice: Who...are...you?"))
	payload += "Alice: Who...are...you?"

	print(list(bytearray(payload)))

	packet = 'V' + struct.pack('>I', len(payload)) + payload

	rabbithole.send(packet)

	if not rx:
		print("Rabbit hole disconnected")
		break