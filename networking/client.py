#Benchmarking RTT

import socket
import time

print "What's the remote host"
HOST = raw_input()         # The host
PORT = 50007               # The same port as used by the server
skt = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
skt.connect((HOST, PORT))

fileStream = open(HOST + '.txt', 'w')
for k in range(0, 100):
  total = 0

  for i in range(0, 100):
    start = time.time()
    skt.send('11111111111111111111111111111111')
    data = skt.recv(32)
    end = time.time()
    total += (end - start)
  
  result = (total) * 1000 / 100
  fileStream.write(str(result) + '\n')

fileStream.close()
skt.close()
