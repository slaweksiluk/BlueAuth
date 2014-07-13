prefix2=/lib/security
prefix=/lib/x86_64-linux-gnu/security

pam.so:
#	gcc -fPIC -c pam.c 
#	gcc -shared -o pam.so pam.o -lpam -lbluetooth
	gcc -fPIC -DPIC -shared -rdynamic -o pam.so pam.c
	gcc -o simplescan simplescan.c -lbluetooth
searchUUID:
	gcc -o searchUUID searchUUID.c -lbluetooth
simplescan:
	gcc -o simplescan simplescan.c -lbluetooth
install: pam.so 
	install -m 755 -o root -g root pam.so $(prefix)
	install -m 755 -o root -g root pam.so $(prefix2)

