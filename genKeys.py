from Crypto.PublicKey import RSA
from Crypto import Random
import pickle

key_len = 1024
form = "PEM"
filename = "keys_obj"

rng = Random.new().read
RSAkey = RSA.generate(key_len, rng)

privatekey = RSAkey
publickey = RSAkey.publickey()
print(privatekey.exportKey(form))  #  export under the 'PEM' format (I think)
print(publickey.exportKey(form))



#  Zapisz obiekt RSAkey do pliku
file_dir = "./keys/" + filename
#    f = open(file_dir, 'w')
#    f.close()
with open(file_dir, 'wb') as output:
    pickle.dump(RSAkey, output, pickle.HIGHEST_PROTOCOL)
