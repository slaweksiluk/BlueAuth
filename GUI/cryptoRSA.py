from Crypto.PublicKey import RSA
from Crypto import Random
import bluetooth
from bluetooth import *
import pickle

def generateKeys(key_len, filename):
    rng = Random.new().read
    RSAkey = RSA.generate(key_len, rng)

    privatekey = RSAkey
    publickey = RSAkey.publickey()
    print(privatekey.exportKey())  #  export under the 'PEM' format (I think)
    print(publickey.exportKey())



#  Zapisz obiekt RSAkey do pliku
    file_dir = "../keys/" + filename
#    f = open(file_dir, 'w')
#    f.close()
    with open(file_dir, 'wb') as output:
        pickle.dump(RSAkey, output, pickle.HIGHEST_PROTOCOL)