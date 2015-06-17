from bluetooth import *

uuid = "00001101-0000-1000-8000-00805F9B34FB"
service_matches = find_service(uuid = uuid)

if len(service_matches) == 0:
    print "couldnt find the mobile!"
first_match = service_matches[0]

port = first_match["port"]
name = first_match["name"]
host = first_match["host"]
print "Found phone: ", host, name, port

