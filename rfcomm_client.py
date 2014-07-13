from bluetooth import *
import bluetooth
bd_addr = "8C:71:F8:99:F0:73"
port = 9


while True:
	nearby_devices = discover_devices()
	for address in nearby_devices:
		print address
		if (bd_addr == address):
			try:
				sock=bluetooth.BluetoothSocket( bluetooth.RFCOMM )
				print "sock created"
				sock.connect((bd_addr, port))
				print "connected"
				while True:
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
						print "write unlock screen file"
						if os.path.exists("/tmp/lock_flag"):
							os.remove("/tmp/lock_flag")
					else:
						print "lock screen file"
						if (os.path.exists("/tmp/lock_flag"))==0:
							f = file("/tmp/lock_flag", "w")
			except BluetoothError as e:
				print "BT error({0}): {1}".format(e.errno, e.strerror)
				sock.close()
				break		
		else:
			print "Phone not responding"
			if (os.path.exists("/tmp/lock_flag"))==0:
				f = file("/tmp/lock_flag", "w")


