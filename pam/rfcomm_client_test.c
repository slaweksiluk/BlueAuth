#include <stdio.h>
#include <sys/time.h>
#include "blue_auth.h"

int main(void){
    struct timeval start, end;
    long mtime, seconds, useconds;    

    int retcode;
    char* user = "heniek";
    char btaddr[18];
    int  port;
	int  key_len;

//czas start
    gettimeofday(&start, NULL);
    retcode = check_user(&user, btaddr, &port, key_len, key);
	char key[key_len];

    printf("check_user() retcode: %d, btaddr: %s, porot: %d, key_len: %d, key: %s\n",retcode, btaddr, port, key_len, k);
    retcode = rfcomm_client(btaddr,port);
    printf("rfcomm_client() retcode: %d\n", retcode);
// czas stop
    gettimeofday(&end, NULL);
    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
    printf("Elapsed time: %ld milliseconds\n", mtime);
    return retcode;
}
