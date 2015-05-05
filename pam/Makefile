prefix=/lib/security


# The name of the object files
OBJS = rfcomm_client.o check_user.o minIni.o
HDRS = ../minini/minIni.h blue_auth.h
# Add -I to the dir the curl include files are in
CFLAGS = -Wall


rfcomm_client_test: $(OBJS) rfcomm_client_test.o $(HDRS) 
	gcc $(CFLAGS) -o rfcomm_client_test $(OBJS) rfcomm_client_test.o -lbluetooth -lm

rfcomm_client_test.o: rfcomm_client_test.c
	gcc $(CFLAGS) -c rfcomm_client_test.c

rfcomm_client.o: rfcomm_client.c 
	gcc $(CFLAGS) -fPIC -c rfcomm_client.c 

check_user.o:  check_user.c
	gcc $(CFLAGS) -fPIC -c check_user.c

minIni.o: ../minini/minIni.c
	gcc $(CFLAGS) -fPIC -c ../minini/minIni.c

pam_blueAuth.o: pam_blueAuth.c
	gcc $(CFLAGS) -fPIC -c pam_blueAuth.c

time_measure: 
	gcc $(CFLAGS) -o time_measure time_measure.c


pam_test_prog:
	gcc -o pam_test_prog pam_test_prog.c -lpam -lpam_misc $(CFLAGS)

pam_blueAuth: $(OBJS) pam_blueAuth.o $(HDRS)
	gcc $(CFLAGS) -fPIC -o pam_blueAuth.so $(OBJS) pam_blueAuth.o -fPIC -DPIC -shared -rdynamic -lbluetooth -lm -lpam -lpam_misc

#pam_blueAuth: $(OBJS) .o $(HDRS)
#	gcc $(CFLAGS) -fPIC -DPIC -shared -rdynamic -o pam_blueAuth.so pam_blueAuth.c 

clean:
	rm -f *.o pam_test_prog

install:
	install -m 755 -o root -g root pam_blueAuth.so $(prefix)

