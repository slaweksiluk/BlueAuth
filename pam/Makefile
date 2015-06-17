prefix=/lib/security

# The name of the object files
OBJS = bt_connection.o crypto_engine.o base64.o ini_config.o minIni.o
HDRS = blue_auth.h minini/minIni.h minini/minGlue.h
# Add -I to the dir the curl include files are in
#CFLAGS = -Wall -fPIC -DPIC -shared -rdynamic
CFLAGS = -Wall -fPIC --shared 

FLAGS = -lbluetooth -pthread -lcrypto -lssl -lpam -lpam_misc -lxss

all: $(OBJS) $(HDRS)

bt_connection.o: bt_connection.c 
	gcc $(CFLAGS) -c bt_connection.c
	
crypto_engine.o: crypto_engine.c
	gcc $(CFLAGS) -c crypto_engine.c
	
base64.o: base64.c
	gcc $(CFLAGS) -c base64.c
	
ini_config.o: ini_config.c
	gcc $(CFLAGS) -c ini_config.c
	
minIni.o: minini/minIni.c
	gcc $(CFLAGS) -c minini/minIni.c
	
	
# BLUE AUTH
pam_blueAuth.o: pam_blueAuth.c
	gcc $(CFLAGS) -c pam_blueAuth.c
	
pam_blueAuth: $(OBJS) pam_blueAuth.o $(HDRS)
	gcc $(CFLAGS) -o pam_blueAuth.so $(OBJS) pam_blueAuth.o  $(FLAGS)
	

# X IDLE
pam_xidle.o: pam_xidle.c
	gcc $(CFLAGS) -c pam_xidle.c
	
pam_xidle: pam_xidle.o
	gcc $(CFLAGS) -o pam_xidle.so pam_xidle.o  $(FLAGS)
	
install:
	install -m 755 -o root -g root pam_blueAuth.so $(prefix)
	install -m 755 -o root -g root pam_xidle.so $(prefix)
		
clean:
	rm -f *.o 
