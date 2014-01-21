from bluetooth import *
import bluetooth
bd_addr = "8c:71:f8:99:f0:73"

port = 9

sock=bluetooth.BluetoothSocket( bluetooth.RFCOMM )
print "sock created"


sock.connect((bd_addr, port))
print "connected"
sock.send("hello!!")
print "message sent"

buf = ""
while True:
	data = sock.recv(1024)
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


sock.close()
