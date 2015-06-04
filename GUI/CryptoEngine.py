from Crypto.PublicKey import RSA
from Crypto import Random
from Crypto.Cipher import PKCS1_OAEP
from base64 import b64decode


key_len = 1024
form = "PEM"
K = "random_value"


def genKeys(pub_dir, priv_dir):
    rng = Random.new().read
    RSAkey = RSA.generate(key_len, rng)

    privatekey = RSAkey
    publickey = RSAkey.publickey()
    print "RAW RSA private key: " + privatekey.exportKey(form, None, 8)  #  export under the 'PEM' format (I think)
    print "RAW RSA public key: " + publickey.exportKey(form)

    pub_file = open(pub_dir, 'w')
    pub_file.write(publickey.exportKey(form))
    pub_file.close()

    priv_file = open(priv_dir, 'w')
    priv_file.write(privatekey.exportKey(form, None, 8))
    priv_file.close()

    return publickey.exportKey(form)

def decryptRSA(ct, file_dir, PKCS, B64):
    key = open(file_dir, "r").read()
    RSAkey = RSA.importKey(key)
    if PKCS:
        RSAkey = PKCS1_OAEP.new(RSAkey)
    if B64:
        to_dec = b64decode(ct)
    print "enecrypted data size: " + str(len(to_dec))
    decrypted = RSAkey.decrypt(to_dec)
    print "decrypted data size: " + str(len(decrypted))
    return decrypted


#run(True, False, False, True, 24)
#ct = "BjWnd4XVqLLqfpj9thMez/f27X4SZqtKm6gY06mE0y1GzndwAl0sPdXlr8lQMZe6B0bM2hZO/J4iJ11NnkC5JFN7Fo1PYJypa6EpAHhXoQsZSQMQhp3wlJPJghpkImjBzgS/A4Ga17DwGO8K2GxJN3DtJpHGAUqbIzLrnZgCMn8="
#runNoBT(False, ct)



