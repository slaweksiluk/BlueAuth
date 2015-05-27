import sys
import os

from subprocess import call
from subprocess import check_output

FIXED_UUID = True
UUID_DIR = "/tmp/uuid.txt"
JAR_DIR = "/home/heniek/j2mewtk/2.5.2/apps/Server/bin/Server.jar"


def createJAR():
    if FIXED_UUID:
        uuid = 'e003ca3e-9975-11e4-ab5b-74e50b44a938'
    else:
        uuid = check_output(["uuidgen", "-t"])

# usun znak konca linii
    uuid = uuid.rstrip()

    uuid_no_hypens = uuid.translate(None, '/r/n -')
    f = open(UUID_DIR, 'w')
    f.write(uuid_no_hypens)
    f.close()

# przekopiowanie UUID do J2ME do testow


# usuiniece pliku uuid z archiwum JAR
    retcode = call(["zip", "-d", JAR_DIR, "uuid.txt"])
    if retcode:
        print "delete failure" + str(retcode)
    else:
        print "delete OK"

    retcode = call(["zip", "-j", "-u", JAR_DIR, UUID_DIR])
    if retcode:
        print "zip update failure" + str(retcode)
        return retcode
    else:
        print "zip update OK"
        return uuid

#def updateConf(arg):
#    f = open('/etc/blueAuth.conf')
#    os.seteuid(0)
