from socket import *
from time import ctime

HOST = ''
PORT = 21567
BUFSIZE = 1024
ADDR = (HOST, PORT)

serverSock = socket(AF_INET, SOCK_STREAM)
serverSock.bind(ADDR)
serverSock.listen(5)

while True:
    print("waiting for connect")
    clientSock, addr = serverSock.accept()
    print("connect from :", addr)

    while True:
        data = clientSock.recv(BUFSIZE)
        if not data :
            break
        clientSock.send("asd")
    clientSock.close()
serverSock.close()
