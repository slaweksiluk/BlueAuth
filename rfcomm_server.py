from bluetooth import *
port = 3
server_sock=BluetoothSocket( RFCOMM )
server_sock.bind(("",port))
print "1"
server_sock.listen(1)
print "2"
client_sock, client_info = server_sock.accept()
print "Accepted connection from ", client_info
#data = client_sock.recv(1024)
#print "received [%s]" % data
client_sock.send("hello!!")
client_sock.close()
server_sock.close()

