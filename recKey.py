from bluetooth import *
import bluetooth
from Crypto.PublicKey import RSA
from Crypto import Random
from Crypto.Cipher import PKCS1_OAEP
import pickle
from base64 import b64decode




#bd_addr = "8C:71:F8:99:F0:73"
#port = 8

bd_addr = "5E:27:46:65:82:AE"
port = 24

key_len = 1024
form = "PEM"
filename = "keys_obj"

KEY_CHARS = 16
data = ""
try: 
	sock=bluetooth.BluetoothSocket( bluetooth.RFCOMM )
	sock.connect((bd_addr, port))

	sock.send("KEY_REQU")
	
#	for i in range(1,KEY_CHARS+1):
#		ch = sock.recv(1)
#		data = data + ch
	s = sock.recv(1024)
	print "received [%s]" % s

#	data = sock.recv(4)
#	print "received [%s]" % data
	
	sock.close();

except BluetoothError as e:
	print "BT error({0}): {1}".format(e.errno, e.strerror)
	sock.close()

# zdekeduj...
# zaladuj klucze z pliku...
file_dir = "./keys/" + filename
with open(file_dir, 'rb') as input_file:
	RSAkey = pickle.load(input_file)
    
RSAkey = PKCS1_OAEP.new(RSAkey)
#privatekey = RSAkey
to_decrypt = b64decode(s)
print "data to decrypt: "
print to_decrypt

decrypted = RSAkey.decrypt(to_decrypt) 
print "decrypted data: " + decrypted



