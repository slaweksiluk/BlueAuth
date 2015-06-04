import sys
import os
from subprocess import call
from subprocess import check_output

def createJAR():
	uuid = check_output(["uuidgen", "-t"])
	f = open('uuid.txt', 'w')
	f.write(uuid)
	f.close()

	retcode = call(["zip", "-r", "Server.jar", "uuid.txt"])
	if retcode:
		print "zip update failure"
		return retcode
	else:
		print "zip update OK"
		return uuid

def updateConf(arg):
	f = open('/etc/blueAuth.conf')
	os.seteuid(0)