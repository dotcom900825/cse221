import socket
import sys

print "What's the file size"
FILE_SIZE = raw_input()
HOST = ''                 
PORT = 50007     
skt = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
skt.bind((HOST, PORT))
skt.listen(1)

while 1:
    conn, addr = skt.accept()
    print 'Connected by', addr
   
    while 1:
        data = conn.recv(5)
        if not data: break
        fileData = '1' * int(FILE_SIZE) * 1024 * 1024
        print 'start to send %d' % sys.getsizeof(fileData)
        conn.send(data)     
        conn.sendall(fileData)
   
    conn.close()
    print 'connect closed'