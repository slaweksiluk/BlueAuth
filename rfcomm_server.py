from bluetooth import *
import os
port = 3
server_sock=BluetoothSocket( RFCOMM )
server_sock.bind(("",port))
print "1"
server_sock.listen(1)
print "2"
client_sock, client_info = server_sock.accept()
print "Accepted connection from ", client_info

client_sock.send("hello!!")
buf = ""
while True:
	data = client_sock.recv(1024)
	buf = buf + data
	if len(buf) == 6: break
	#print "received [%s]" % data
print "recived [%s]" % buf

if buf == "unlock":
	print "unlock screen"
	if os.path.exists("/tmp/lock_flag"):
		os.remove("/tmp/lock_flag")
else:
	print "lock screen"
	if (os.path.exists("/tmp/lock_flag"))==0:
		f = file("/tmp/lock_flag", "w")

client_sock.close()
server_sock.close()

