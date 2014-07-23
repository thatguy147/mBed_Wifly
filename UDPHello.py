import socket
import time

mySocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
mySocket.bind(('192.168.1.11',6000))
for x in range (0, 32675):
    mySocket.connect_ex(('192.168.1.1',6001))
    mySocket.send ('Wherefore art though?')
    time.sleep(1)
    
