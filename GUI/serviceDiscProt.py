import sys
from bluetooth import *

def findPhone(uuid):

#f = open('/etc/blueAuth.conf', 'r+')
#print f
#uuid  = f.readline()
#uuid  = f.readline()
#uuid = uuid.rstrip()
    print uuid


#uuid = "95cb7094-0a7c-11e4-b95c-485b391c6106"
    service_matches = find_service( uuid = uuid )

    if len(service_matches) == 0:
            raise ValueError('findPhone() error: cannot find mobile with UUID ', uuid)
            return None

    first_match = service_matches[0]
    port = first_match["port"]
    name = first_match["name"]
    host = first_match["host"]
    print "connecting to ", host, name, port

    return (host, port)

def listDev():
	nearby_devices = discover_devices()
	for bdaddr in nearby_devices:
		print "found target bluetooth device with address "
		print bdaddr
		print lookup_name( bdaddr )
		services = find_service("fa87c0d0-afac-11de-8a390800200c9a66")
		print services

def sendIDKey(host, port):
    try:
        sock=bluetooth.BluetoothSocket(bluetooth.RFCOMM)
        sock.connect((host, port))
        IDkey = "f893253a9f4d11e491c374e50b44a938"
        sock.send(IDkey)
    except BluetoothError as e:
        print "sendIDkey(): error({0}): {1}".format(e.errno, e.strerror)
        sock.close()

def sendIDKey(host, port):
    try:
        sock=bluetooth.BluetoothSocket(bluetooth.RFCOMM)
        sock.connect((host, port))
        IDkey = "f893253a9f4d11e491c374e50b44a938"
        sock.send(IDkey)
    except BluetoothError as e:
        print "sendIDkey(): error({0}): {1}".format(e.errno, e.strerror)
        sock.close()

#f.write(host)
#f.write("\n")
#f.write(str(port))
#f.write("\n")
#f.close()

#sock=BluetoothSocket( RFCOMM )
#sock.connect((host, port))
#sock.send("hello!!")
#sock.close();

