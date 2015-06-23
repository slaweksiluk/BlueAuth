# -*- coding: utf-8 -*-
import bluetooth
import socket
import time

MSG_LEN = 8
CT_LEN = 172
"""
Created on Thu May 28 21:25:58 2015

@author: slawek
"""


def sendPubKey(btaddr, port, pubKey):
    # Wyslanie klucza publicznego do telefonu
    sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
    try:
        sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
        sock.connect((btaddr, port))
        sock.setblocking(True)
        bytes = sock.send("PUBL_KEY", socket.MSG_WAITALL)
        print "sent bytes: " + str(bytes) 
        bytes = sock.send(pubKey, socket.MSG_WAITALL)
        print "sent bytes: " + str(bytes) 
	time.sleep(1)
        sock.close()
    except bluetooth.BluetoothError as e:
        print "BT error: " + e.message
        sock.close()


def receiveCipherText(btaddr, port, sessionID):
    # Telefon zamkna te polaczanie. Trzeba czekac na utowrzenie nastepnego,
    # a nastapi to gdy zostanie wprowadzone haslo
    try:
        #sock.setblocking(True)
        sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
        sock.connect((btaddr, port))
        sock.setblocking(True)

        # Wyslij AUTH REQUES, aby otrzymac klucz identyfikujacy...
        sock.send("AUTH_REQ")
        print "sent: AUTH_REQ"

        # Wyslij id sesji
        sock.send(sessionID)
        print "sent: " + sessionID

        sock.setblocking(True)
        # Odbierz odpowiedz1
        data = sock.recv(MSG_LEN, socket.MSG_WAITALL)
        print "received: " + data
        if data != "NON_CONF":
            print "Unknown control msg!"

        sock.setblocking(True)
        # Odbierz odpowiedz2
        data = sock.recv(MSG_LEN, socket.MSG_WAITALL)
        print "received: " + data
        if data != "AUT_PERM":
            print "Unknown control msg!"

        sock.setblocking(True)
        # Odbierz szyfrigram
        data = sock.recv(CT_LEN, socket.MSG_WAITALL)
        print "received: " + data

        sock.close()
        return data

    except bluetooth.BluetoothError as e:
        print "BT error: " + e.message
        sock.close()
        return None

def findPhone(uuid):
    print("Looking for mobile with UUID: %s") % uuid
    service_matches = bluetooth.find_service(uuid=uuid)
    if len(service_matches) == 0:
        raise ValueError('findPhone() error: cannot find mobile with UUID ',
                         uuid)
        return None

    first_match = service_matches[0]
    port = first_match["port"]
    name = first_match["name"]
    host = first_match["host"]
    print "Found mobile: " + name + " addr: " + host + " port: " + str(port)
    return (host, port)
