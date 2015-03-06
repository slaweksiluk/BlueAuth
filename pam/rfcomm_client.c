//Example 3-3. rfcomm-client.c
//gcc rfcomm_client.c -o rfcomm_client  -lbluetooth
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include "blue_auth.h"



/*int main(int argc, char **argv)*/
int rfcomm_client(const char *dest, const int port)
{

  //char *dest = NULL;
  //size_t len = 0;
  //int dest_len; //

	struct sockaddr_rc addr = { 0 };
	int status;
	int s;
	int key_len = 16  // 16 znakow 
	char key[] = "f893253a9f4d11e491c374e50b44a938";
	char *buf;
//	char *compare;

//removes newline character
//dest_len = strlen(dest);
//if( dest[dest_len-1] == '\n' )
//    dest[dest_len-1] = 0;

	//read_s = getline(&port, &len, fp); //port
//int port_int = atoi( port );
            //printf("%d", port_int);



	//compare = "u";
	//char dest[18] = "8c:71:f8:99:f0:73";
	printf("allocate a socket\n");
	s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
	// set the connection parameters (who to connect to)
	addr.rc_family = AF_BLUETOOTH;
	addr.rc_channel = (uint8_t) port;
	str2ba( dest, &addr.rc_bdaddr );
	printf("connect to server\n");
	status = connect(s, (struct sockaddr *)&addr, sizeof(addr));

	// send a message
	if( status == 0 ) {
		status = write(s, "KEY_REQ", 7);
		if( status < 0 ) {
			perror("write error\n");
			close(s);
			return -1;
		}
		while(status >= 0){ 
			status = read(s, buf, sizeof(buf));
			printf("rfcomm_client() recived: %s, status: %d \n", buf, status);
		}

		if( status < 0 ){
			perror("rfcomm_client() read error\n");
			close(s);
			return -1;
		}
		if(strcmp(buf,key) == 0){
			return 0;
		}
		else{
			return -1;
		}
		//}
	}
	else{
		perror("connect error\n");
		return -1;
	}
	printf("return: -1\n");
	return -1;
}


