from bluetooth import *
target_name = "Wave533"
target_address = None
nearby_devices = discover_devices()
for address in nearby_devices:
	if target_name == lookup_name( address ): 
		target_address = address 
	else:
		break

if target_address is not None:
	print "found target bluetooth device with address ", target_address
else:
	print "could not find target bluetooth device nearby"

