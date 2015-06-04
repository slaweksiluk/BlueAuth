from Crypto.PublicKey import RSA
from base64 import b64decode
from Crypto import Random

key_len = 1024
form = "PEM"
K = "random_value"
filename_pub = "pub.pem"
filename_priv = "priv.pem"
file_dir = "keys/"

def gen_keys():
	rng = Random.new().read
	RSAkey = RSA.generate(key_len, rng)
	
	privatekey = RSAkey
	publickey = RSAkey.publickey()
	print "RAW RSA private key: " + privatekey.exportKey(form)  #  export under the 'PEM' format (I think)
	print "RAW RSA public key: " + publickey.exportKey(form)
	
	pub_file = open(file_dir + filename_pub, 'w')
	pub_file.write(publickey.exportKey(form))
	pub_file.close()
	
	priv_file = open(file_dir + filename_priv, 'w')
	priv_file.write(privatekey.exportKey(form))
	priv_file.close()
	
	
def encrypt_RSA(message):
    '''
    param: public_key_loc Path to public key
    param: message String to be encrypted
    return base64 encoded encrypted string
    '''
    
    key = open((file_dir + filename_pub), 'r').read()
    RSAkey = RSA.importKey(key)
    
    encrypted = RSAkey.encrypt(message, K)
    print "encrypted data: "
    print encrypted
    return encrypted
    
def decrypt_RSA(package):
    '''
    param: public_key_loc Path to your private key
    param: package String to be decrypted
    return decrypted string
    '''
    key = open(file_dir + filename_priv, "r").read()
    RSAkey = RSA.importKey(key)
    
    decrypted = RSAkey.decrypt(package)
    return decrypted
    

gen_keys()
msg = "hello!"
data = encrypt_RSA(msg)
#print "encrypted: " + data
data = decrypt_RSA(data)
print "decrypted: " + data

