from bluetooth import *
import bluetooth
from Crypto.PublicKey import RSA
from Crypto import Random
import pickle

#bd_addr = "8C:71:F8:99:F0:73"
#port = 8

bd_addr = "5E:27:46:65:82:AE"
port = 24

data_rec = ""
key_len = 1024
form = "PEM"
filename = "keys_obj"

#rng = Random.new().read
#RSAkey = RSA.generate(key_len, rng)
#RSAkey = None;

# zaladuj klucze z pliku...
file_dir = "./keys/" + filename
with open(file_dir, 'rb') as input_file:
	RSAkey = pickle.load(input_file)



#privatekey = RSAkey
publickey = RSAkey.publickey()
#print(privatekey.exportKey())  #  export under the 'PEM' format (I think)
print(publickey.exportKey(form))

try: 
	sock=bluetooth.BluetoothSocket( bluetooth.RFCOMM )
	sock.connect((bd_addr, port))
	#key_s = "f893253a9f4d11e491c374e50b44a938" #32 znaki
	#key_s = "f893253a9f4d11e2" #16 znaki
	#  Najpierw przeslanie dlugosci klucz
	sock.send("KEY_1024")
	sock.send(publickey.exportKey(form))
	sock.close()
	
	#  1 - 32bit
	#  2 - 6
	#sock.send(key_s)
	#sock.close()

#data = sock.recv(32)
#print "received [%s]" % data


except BluetoothError as e:
	print "BT error({0}): {1}".format(e.errno, e.strerror)
	#sock.close()

