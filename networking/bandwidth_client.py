import socket
import time
import sys

print "What's the host name"
HOST = raw_input()
print "What's the file size"
FILE_SIZE = raw_input()
PORT = 50007               

skt = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
skt.connect((HOST, PORT))
skt.send('start')
skt.recv(5)

print 'start data'
total_time = 0
data = ''
fileSize = int(FILE_SIZE) * 1024 * 1024

while (sys.getsizeof(data) < fileSize):
    start = time.time()
    data += skt.recv(fileSize)
    end = time.time()
    total_time += end - start

result = total_time * 1000     
dataSize = sys.getsizeof(data)
print dataSize
average = dataSize / result      #bandwidth
print '%.2f %.2f' % (result, average)
skt.close()
