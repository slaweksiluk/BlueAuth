from Crypto.PublicKey import RSA
from Crypto import Random
from Crypto.Cipher import PKCS1_OAEP
from base64 import b64decode

from bluetooth import *
import bluetooth

key_len = 1024
form = "PEM"
K = "random_value"

uuid = "00001101-0000-1000-8000-00805F9B34FB"
bd_addr = "5E:27:46:65:82:AE"

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
	
def findPhone(uuid):
    service_matches = find_service(uuid = uuid)

    if len(service_matches) == 0:
        print "couldnt find the mobile!"
    first_match = service_matches[0]

    port = first_match["port"]
    name = first_match["name"]
    host = first_match["host"]
    print "Found phone: ", host, name, port
    return port

def sendPubKey(port, bd_addr, file_dir):
# zaladuj klucze z pliku...
    key = open(file_dir, 'r').read()
    RSAkey = RSA.importKey(key)
    publickey = RSAkey.publickey()
    print "sentKey(): public key from file"
    print(publickey.exportKey(form))
    
    data_rec = ""
    try: 
	    sock=bluetooth.BluetoothSocket( bluetooth.RFCOMM )
	    sock.connect((bd_addr, port))
	    sock.send("KEY_1024")
	    sock.send(publickey.exportKey(form))
	    print "sendPubKey(): success"
	    sock.close()

    except BluetoothError as e:
	    print "BT error({0}): {1}".format(e.errno, e.strerror)
	    sock.close()
	    
def recIDKey(port, bd_addr):

    data = ""
    try: 
	    sock=bluetooth.BluetoothSocket( bluetooth.RFCOMM )
	    sock.connect((bd_addr, port))
	    sock.send("KEY_REQU")
	    ct = sock.recv(2048)
	    print "received ciphertext:"
	    print ct
	    sock.close();

    except BluetoothError as e:
	    print "BT error in recIDKEY ({0}): {1}".format(e.errno, e.strerror)
	    sock.close()
	    
    return ct
	    
	    
def decryptRSA(ct, file_dir, PKCS, B64):
    key = open(file_dir, "r").read()
    RSAkey = RSA.importKey(key)
    if PKCS:
        RSAkey = PKCS1_OAEP.new(RSAkey)
    if B64:
        to_dec = b64decode(ct)
    print "decrypted data size before decrypt: " + str(len(to_dec))
    print "decrypted data before decrypt: "
    print to_dec
    decrypted = RSAkey.decrypt(to_dec)
    print "decrypted data: "
    print decrypted
	    
def run(GEN, SEND, PKCS, B64, port):
    pub_dir = "keys/pub.pem"
    priv_dir = "keys/priv.pem"
    
    if GEN:
        genKeys(pub_dir, priv_dir)
    
    #port = findPhone(uuid)
    if SEND:
        sendPubKey(port, bd_addr, pub_dir)
        
    rec_data = recIDKey(port, bd_addr)
    decryptRSA(rec_data, priv_dir, True, True)
    
def runNoBT(GEN, to_decrypt):
    priv_dir = "keys/priv.pem"
    if GEN:
        genKeys(pub_dir, priv_dir)
        
    decryptRSA(to_decrypt, priv_dir, True, True)
    
    
#run(True, False, False, True, 24)
ct = "XkErXjcguh5ZrffTrXJPlPW0S8FtWNUG3FCQY/47reUtDNgoHPP9CimqrN/pJ678I2MoUoHQdRYE9AU+AO5STuiArYZscRKKuc4dP+j4k46t4Q/I7gexCuu1W2Ki04m5qbUGGjvnkeZKjEd18IEDHDcJOn8iUBhjqOF0rQFUpVA="
runNoBT(False, ct)



