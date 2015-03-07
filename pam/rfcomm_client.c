//Example 3-3. rfcomm-client.c
//gcc rfcomm_client.c -o rfcomm_client  -lbluetooth
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> // malloc
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include "blue_auth.h"



/*int main(int argc, char **argv)*/
int rfcomm_client(const char *dest, const int port, const int key_len, char *key_rec)
{

  //char *dest = NULL;
  //size_t len = 0;
  //int dest_len; //

	struct sockaddr_rc addr = { 0 };
	int status;
	int s;
	char key[key_len +1];
	char *c = (char*) malloc(sizeof(char));

	s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
	addr.rc_family = AF_BLUETOOTH;
	addr.rc_channel = (uint8_t) port;
	str2ba( dest, &addr.rc_bdaddr );
	printf("rfcomm_client() establishing connection...\n");
	status = connect(s, (struct sockaddr *)&addr, sizeof(addr));

	// send a message
	if( status == 0 ) {
		status = write(s, "KEY_REQ", 7);
		if( status < 0 ) {
			perror("write error\n");
			close(s);
			return -1;
		}
		printf("rfcomm_client() key_len = %d\n", key_len);
		int i = 0; 
		for(i = 0; i < key_len; i++){ 
			status = read(s, c, 1);
			//printf("rfcomm_client() recived: %s, status: %d \n", c, status);
			key[i] = *c; 

		}
// znak zakonczenia napisu
		key[key_len] = '\0';
		printf("rfcomm_client() recived key: %s, status: %d \n", key, status);

		if( status < 0 ){
			perror("rfcomm_client() read error\n");
			close(s);
			return -1;
		}
        strcpy(key_rec, key);
		return 0;
	}
	else{
		perror("rfcomm_client(): connect error\n");
		return -1;
	}
}


