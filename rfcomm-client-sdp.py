import sys
from bluetooth import *

f = open('/etc/blueAuth.conf', 'r+')
print f
uuid  = f.readline()
uuid  = f.readline()
uuid = uuid.rstrip()
print uuid


#uuid = "95cb7094-0a7c-11e4-b95c-485b391c6106"
service_matches = find_service( uuid = uuid )

if len(service_matches) == 0:
	print "couldnt find the service!"
	sys.exit(0)
first_match = service_matches[0]
port = first_match["port"]
name = first_match["name"]
host = first_match["host"]
print "connecting to ", host, name, port


f.write(host)
f.write("\n")
f.write(str(port))
f.write("\n")
f.close()

sock=BluetoothSocket( RFCOMM )
sock.connect((host, port))
sock.send("hello!!")
sock.close();

