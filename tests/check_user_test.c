#include <stdio.h>
#include "blue_auth.h" 


int main(void){
    int retcode;
    char* user = "heniek";
    char* btaddr;
    int  port;

    retcode = check_user(&user, &btaddr, &port);

    printf("btaddr: %s, porot: %d\n", btaddr, port);

    return retcode;
}
