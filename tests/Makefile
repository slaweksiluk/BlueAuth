prefix=/lib/security


# The name of the object files
#OBJS = bt_connection.o bt_connection_test.o
#HDRS = ../pam/blue_auth.h
# Add -I to the dir the curl include files are in
#CFLAGS = -Wall -fPIC
CFLAGS = -Wall
FLAGS = -lbluetooth -pthread -lcrypto -lssl


# Obiekty 
OBJS = $(patsubst %.o,../pam/%.o,bt_connection.o crypto_engine.o base64.o ini_config.o minIni.o)
HDRS = ../pam/blue_auth.h ../pam/minini/minIni.h ../pam/minini/minGlue.h 



all: 
	+$(MAKE) -C ../pam
	+$(MAKE) bt_connection_test
	+$(MAKE) crypto_engine_test
	+$(MAKE) ini_config_test
	+$(MAKE) pam_test_prog

# BT_CONNECTION_TEST
bt_connection_test:  bt_connection_test.o $(OBJS) $(HDRS) 
	gcc $(CFLAGS) -o bt_connection_test  bt_connection_test.o $(OBJS) $(FLAGS)
bt_connection_test.o: bt_connection_test.c
	gcc $(CFLAGS)  -c bt_connection_test.c

# CRYPTO_ENGINE_TEST
crypto_engine_test: crypto_engine_test.o $(OBJS) $(HDRS) 
	gcc $(CFLAGS) crypto_engine_test.o $(OBJS) -o crypto_engine_test $(FLAGS)
crypto_engine_test.o: crypto_engine_test.c
	gcc $(CFLAGS) -c crypto_engine_test.c 

# INI_CONFIG_TEST
ini_config_test: ini_config_test.o $(OBJS) $(HDRS)
	gcc $(CFLAGS) ini_config_test.o $(OBJS) -o ini_config_test $(FLAGS)
ini_config_test.o: ini_config_test.c
	gcc $(CFLAGS) -c ini_config_test.c
	
# PAM_TEST_PROG
pam_test_prog:
	gcc -o pam_test_prog pam_test_prog.c -lpam -lpam_misc $(CFLAGS)
	
clean:
	rm -f *.o 

