//gcc -Wall -o thread_test thread_test.c -pthread
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


pthread_mutex_t count_mutex     = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  condition_var   = PTHREAD_COND_INITIALIZER;


void *delay_fun(void *delay)
{
   //int *del_ptr, del;
   //del_ptr = (int*)delay;
   //del = *(int*)del_ptr;

   int del;
   del = *(int*)delay;
     
   printf("This is sleep thread, slepp time: %d\n", del);
   sleep(del);

   printf("This is sleep thread, sending signal...\n");  
   pthread_mutex_lock( &count_mutex );
   pthread_cond_signal( &condition_var );
   pthread_mutex_unlock( &count_mutex );   
   pthread_exit(NULL);
}


void *conn_fun(void *login)
{
   char *log = (char*)login;
   int *ret_ptr, ret;
   ret = 88;
   ret_ptr = &ret;   
     
   printf("This is conn thread, user login: %s\n", log);
   printf("This is conn thread, connecting to mobile...\n");   
   sleep(3);
   printf("This is conn thread, connected! Received USR_CONF...\n");   
   //printf("In conn thread: check for signal\n");   
   //pthread_cond_wait( &condition_var, &count_mutex );
   //printf("In conn thread: received time out signal\n");
   
   //printf("This is conn thread, sending signal...\n");  
   //pthread_mutex_lock( &count_mutex );
   //pthread_cond_signal( &condition_var );
   //pthread_mutex_unlock( &count_mutex );
   
   pthread_exit(&ret);
}

int main (int argc, char *argv[])
{
    pthread_t count_th;
    pthread_t conn_th;
    int rc;

    int *delay_ptr, delay;
    delay = 10;
    delay_ptr = &delay;
    
    char login[] = "user_login";
    void *status;
    

    
    printf("In main: creating thread conn_th\n");
    rc = pthread_create(&conn_th, NULL, conn_fun,(void *)login);
    if (rc){
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }
    
    //printf("In main: check for signal\n");   
    //pthread_cond_wait( &condition_var, &count_mutex );
    //printf("In main: received signal and joining conn thread\n");
    //sleep(10);
    printf("In main: joining conn thread...\n");   
    rc = pthread_join(conn_th, &status);
    if (rc) {
       printf("ERROR; return code from pthread_join() is %d\n", rc);
       exit(-1);
    }
    
    int status_int;
    status_int = *(int*)status;
    printf("Status from conn: %d\n", status_int);
    
   /* Last thing that main() should do */
   pthread_exit(NULL);
}
