from bluetooth import *
import bluetooth
bd_addr = "8c71f899f073"

port = 3

sock=bluetooth.BluetoothSocket( bluetooth.RFCOMM )
print "sock created"


sock.connect((bd_addr, port))
print "connected"

sock.recv(1024)
print "buffer recived"
#data = sock.recv(1024)
#print "received [%s]" % data

sock.send("hello!!")
print "message send"

sock.close()
