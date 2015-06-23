#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/scrnsaver.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "xidle.h"


int det_user_interrupt(const int sample_period, const int debounces, 
        bool verbose){
        //const static int TIME_DIFF_MSECONDS = 0.5;
    int time1;
    int time2;
    int diff;
    int counter = 0;
    //sleep(5);
    //if(putenv("DISPLAY=:0") != 0){
    //    printf("putenv() error:  %s\n", strerror(errno));
    //    return -1;
    //}
    Display *dpy = XOpenDisplay(NULL);
    if (!dpy) {
        if(verbose) printf("err: display NULL\n");
        return(-1);
    }
    
    XScreenSaverInfo *info = XScreenSaverAllocInfo();
    if (!info) {
        if(verbose) printf("err: info NULL\n");
        return(-1);
    }

    while(1){
        //pierwszy pomiar
        time1 = GetIdleTime(dpy, info);
        //przerwa
        usleep(sample_period*1000);
        //drugi pomiar
        time2 = GetIdleTime(dpy, info);
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
    
    if(dpy != NULL) XFree(dpy);
    if(info != NULL) XFree(info);
    if(verbose) printf("cleaned...\n");
    return 0;
}




int GetIdleTime(Display *dpy, XScreenSaverInfo *info){
    XScreenSaverQueryInfo(dpy, DefaultRootWindow(dpy), info);
    return (int)info->idle;
}

