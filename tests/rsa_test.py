from Crypto.PublicKey import RSA
from Crypto.Cipher import PKCS1_OAEP
from base64 import b64decode
from Crypto import Random

key_len = 1024
form = "PEM"

def gen_keys():
	rng = Random.new().read
	RSAkey = RSA.generate(key_len, rng)
	
	privatekey = RSAkey
	publickey = RSAkey.publickey()
	print "RAW RSA private key: " + privatekey.exportKey(form)  #  export under the 'PEM' format (I think)
	print "RAW RSA public key: " + publickey.exportKey(form)


	RSAkey = PKCS1_OAEP.new(RSAkey)
	#privatekey = RSAkey
	#publickey = RSAkey.publickey()
	print "PKCS RSA private key: " + privatekey.exportKey(form)  #  export under the 'PEM' format (I think)
		print "PKCS RSA private key: " + privatekey.exportKey(form)  #  export under the 'PEM' format (I think)
	#print "PKCS RSA public key: " + publickey.exportKey(form)
	
	return RSAkey
	
def encrypt_RSA(RSAkey, message):
    '''
    param: public_key_loc Path to public key
    param: message String to be encrypted
    return base64 encoded encrypted string
    '''
    encrypted = RSAkey.encrypt(message)
    return encrypted.encode('base64')
    
def decrypt_RSA(RSAkey, package):
    '''
    param: public_key_loc Path to your private key
    param: package String to be decrypted
    return decrypted string
    '''
    decrypted = RSAkey.decrypt(b64decode(package)) 
    return decrypted
    
key = gen_keys()
msg = "hello!"
data = encrypt_RSA(key, msg)
print "encrypted: " + data
data2 = decrypt_RSA(key, data)
print "decrypted: " + data2 
