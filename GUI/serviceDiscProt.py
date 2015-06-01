import sys
from bluetooth import *



# Odszukaj telefon z uuuid konczacym sie na
def findMobile(uuid_suf):
    print "Looking for a moble with UUID suffix: " + uuid_suf

#    nearby_devices = discover_devices()
#    for bdaddr in nearby_devices:


    services = find_service()

    if len(services) > 0:
        print("found  services on %s" % (len(services)))
    else:
        print("no services found")

    for svc in services:
        print("Service Name: %s"    % svc["name"])
        print("    Host:        %s" % svc["host"])
        print("    Description: %s" % svc["description"])
        print("    Provided By: %s" % svc["provider"])
        print("    Protocol:    %s" % svc["protocol"])
        print("    channel/PSM: %s" % svc["port"])
        print("    svc classes: %s "% svc["service-classes"])
        print("    profiles:    %s "% svc["profiles"])
        print("    service id:  %s "% svc["service-id"])
        print()

def findPhone(uuid):

    print("Looking for mobile with UUID: %s") % uuid
    service_matches = find_service(uuid=uuid)
    if len(service_matches) == 0:
            raise ValueError('findPhone() error: cannot find mobile with UUID ', uuid)
            return None

    first_match = service_matches[0]
    port = first_match["port"]
    name = first_match["name"]
    host = first_match["host"]
    print("Found mobile %s. BlueAuth service addr %s, port %d") % name % host % port
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

