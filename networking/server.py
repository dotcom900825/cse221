import socket

HOST = ''                 # Symbolic name meaning the local host
PORT = 50007              # Arbitrary non-privileged port
skt = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
skt.bind((HOST, PORT))
skt.listen(1)
while 1:
    conn, addr = skt.accept()
    print 'Connected by', addr
    
    while 1:
        data = conn.recv(64)
        if not data: break
        conn.send(data)
    
    conn.close()
    print 'connect closed'
