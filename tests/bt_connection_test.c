// Test funkcji rfcomm_client() z modulu pam/bt_connection.c
#include <sys/time.h>
#include "../pam/blue_auth.h"


int main(){
    // Pomiar czasu
    struct timeval start, end;
    long mtime, seconds, useconds;    
    // Dane do polacznia
    char *addr = "8C:71:F8:99:F0:73";
    int port = 8;
    // Przykladowy id sesjii
    char* sessionID = "6j7ehX7Dilo=";
    // Dopuszczalny czas odpowiedzi - 20s
    int time_out = 20;
    //  return code
    int rc;    
    // Odebrany szyfrogram
    char ct[CIPHER_TEXT_LEN_B64];
    // Zmierzona wartosc rssi
    float rssi_result;
    //czas start
    gettimeofday(&start, NULL);
    rc = rfcomm_client(addr, port, ct, sessionID, time_out, &rssi_result);
    // Czas stop
    gettimeofday(&end, NULL);
    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    mtime = ((seconds) * 1000 + useconds/1000.0);
    printf("Elapsed time: %ld milliseconds\n", mtime);
    if(rc != 0){
        print_error(rc);
        return rc;
    } else{
        printf("RSSI measurement result: %f\n", rssi_result);
        printf("Received ciphertext: %s\n", ct);
        return 0;
    }
}
