import socket
import time

print "What's the remote host"
HOST = raw_input()         # The host
PORT = 50007               # The same port as used by the server

for i in range(0, 100):
    skt = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    start = time.time()
    skt.connect((HOST, PORT))
    end = time.time()
    print (end - start) * 1000
    skt.close()
