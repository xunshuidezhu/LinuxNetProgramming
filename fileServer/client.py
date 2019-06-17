from socket import *

HOST = 'localhost'
PORT = 21567
BUFFERSIZE = 1024
ADDR = (HOST, PORT)

clientSock = socket(AF_INET, SOCK_STREAM)
clientSock.connect(ADDR)

while True:
    data = input('> ')
    if not data:
        break
    clientSock.send(data)
    data = clientSock.recv(BUFFERSIZE)
    if not data:
        break
    print("asd")
clientSock.close()
