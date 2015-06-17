# -*- coding: utf-8 -*-
"""
Created on Sun May 31 23:58:05 2015

@author: slawek
"""

from CryptoEngine import decryptRSA
import base64
import hashlib

CONS_UUID = "f893253a-9f4d-11e4-91c3-74e5"


def preparePublicKey(pubKey):
    pub_key_temp = pubKey.replace("-----BEGIN PUBLIC KEY-----\n", "")
    pub_key_to_send = pub_key_temp.replace("-----END PUBLIC KEY-----", "").rstrip()
    return pub_key_to_send

def processCipherText(ct, sessionID_B64, priv_key_dir):
    decrypted = decryptRSA(ct, priv_key_dir, True, True)
    rec_sessionID_B64 = base64.b64encode(decrypted[:8])
    print "received sessionID: "
    print rec_sessionID_B64
    if rec_sessionID_B64 != sessionID_B64:
        print "ERROR: diffrents sesion IDs!"
        return None
    rec_keyID = decrypted[8:40]
    print "Received key ID length: " + str(len(rec_keyID))
    print "Receivied decrypted: "
    print ":".join("{:02x}".format(ord(c)) for c in decrypted)

    key_id_hash_b64 = base64.b64encode(hashlib.sha512(rec_keyID).digest())
    print "hashed key id: "
    print key_id_hash_b64
    return key_id_hash_b64

