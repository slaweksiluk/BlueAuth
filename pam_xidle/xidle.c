#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/scrnsaver.h>
#include "xidle.h"

int det_user_interrupt(const int sample_period, const int debounces, 
        bool verbose){
        //const static int TIME_DIFF_MSECONDS = 0.5;
    int time1;
    int time2;
    int diff;
    int counter = 0;

    while(1){
        //pierwszy pomiar
        time1 = GetIdleTime();
        //przerwa
        usleep(sample_period*1000);
        //drugi pomiar
        time2 = GetIdleTime();
        //byl ruch
        diff = time2 - time1;
        if(diff < 0){
            counter = counter + 1;
            if(verbose) printf("counter set to: %d\n", counter);
        }
        if(counter == debounces) 
            break;
    }
    if(verbose) printf("loop broken...\n");
    if(verbose) printf("time1 is: %d\n", time1);
    if(verbose) printf("time2 is: %d\n", time2);
    if(verbose) printf("diff is: %d\n", diff);
    return 0;
}



int GetIdleTime () {
    time_t idle_time;
    static XScreenSaverInfo *mit_info;
    Display *display;
    int screen;
    mit_info = XScreenSaverAllocInfo();
    if((display=XOpenDisplay(NULL)) == NULL) { return(-1); }
    screen = DefaultScreen(display);
    XScreenSaverQueryInfo(display, RootWindow(display,screen), mit_info);
    idle_time = (mit_info->idle);
    XFree(mit_info);
    XCloseDisplay(display); 
    return idle_time;
}

