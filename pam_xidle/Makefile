prefix=/lib/security

# The name of the object files
OBJS = xidle.o pam_xidle.o
HDRS = xidle.h
# Add -I to the dir the curl include files are in
#CFLAGS = -Wall -fPIC -DPIC -shared -rdynamic
CFLAGS = -Wall -fPIC --shared 

FLAGS = -lpam -lpam_misc
XFLAGS = -lX11 -lXss
all:
	+$(MAKE) xidle_test
	+$(MAKE) pam_xidle

# xidle
xidle.o: xidle.c
	gcc $(CFLAGS) -c xidle.c
xidle_test: xidle_test.o xidle.o $(HDRS) 
	gcc -Wall xidle_test.o xidle.o -o xidle_test $(XFLAGS)
xidle_test.o: xidle_test.c
	gcc -Wall -c xidle_test.c
	
# pam_xidle
pam_xidle.o: pam_xidle.c
	gcc $(CFLAGS) -c pam_xidle.c
pam_xidle: $(OBJS)
	gcc $(CFLAGS) $(OBJS) -o pam_xidle.so $(FLAGS) $(XFLAGS)
install:
	install -m 755 -o root -g root pam_xidle.so $(prefix)		
clean:
	rm -f *.o 
