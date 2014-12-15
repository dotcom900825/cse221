import socket
import time

print "What's the remote host"
HOST = raw_input()         # The host
PORT = 50007               # The same port as used by the server
skt = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

for i in range(0, 100): 
    skt.connect((HOST, PORT))
    start = time.time()
    skt.close()
    skt = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    end = time.time()
    print (end - start) * 1000
