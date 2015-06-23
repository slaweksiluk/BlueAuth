#include<stdio.h>
#include<unistd.h>
#include<stdbool.h>
#include"xidle.h"

int main(){
//    printf("Sleep 1s...\n");
//    sleep(1);
//    printf("Test GetIdleTime()\n");
//    int idle_time = GetIdleTime();
//    printf("GetIdleTime() returned idle_time(miliseconds): %d\n", idle_time);
//    
//    printf("Sleep 1s...\n");
//    usleep(1000);
    printf("Test det_user_interrupt(...)\n");
    det_user_interrupt(50, 3, true);
    
    return 0;
}