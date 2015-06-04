#include <stdio.h>
#include "../pam/blue_auth.h" 

int main(){
    int retcode;
    const char* user = "slawek";
    char btaddr[18];
    int  port;
    char mobile[50];
    float rssi;

    retcode = get_user_config(&user, btaddr, &port, mobile, &rssi);
    
    if(retcode < 0){
        print_error(retcode);
        return retcode;
    }

    printf("btaddr: %s, porot: %d\n", btaddr, port);

    return retcode;
}
