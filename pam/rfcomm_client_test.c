#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include "blue_auth.h"

int main(void){
    struct timeval start, end;
    long mtime, seconds, useconds;    

    int retcode;
    const char* user = "heniek";
    char btaddr[18];
    int  port;
	int  key_len;
	
	print_error(E_BT_COMM_SUCCESS);


//czas start
    gettimeofday(&start, NULL);

	key_len = get_key_len(&user);
    printf("get_key_len() key_len: %d\n", key_len);

	char key_cfg[key_len];
    retcode = check_user(&user, btaddr, &port, key_cfg);
    printf("check_user() retcode: %d, btaddr: %s, porot: %d, key: %s\n",retcode, btaddr, port, key_cfg);


	char key_rec[key_len];
    retcode = rfcomm_client(btaddr,port, key_len, key_rec);
    printf("rfcomm_client() retcode: %d, Received: %s\n", retcode, key_rec);

//  Porownanie kluczy
	if(strcmp(key_cfg, key_rec)) printf("Keys equal!\n");
	else printf("Keys NOT equal!\n");

// czas stop
    gettimeofday(&end, NULL);
    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
    printf("Elapsed time: %ld milliseconds\n", mtime);
    return retcode;
}
