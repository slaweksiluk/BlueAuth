//gcc -Wall ioctl_test.c -o ioctl_test -lbluetooth
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> // malloc
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <stdbool.h>
#include <sys/ioctl.h>




/*int main(int argc, char **argv)*/
int main(){
    int key_len = 16;
	const static bool FIXED = true;
	//wave
	char *dest_fixed = "8C:71:F8:99:F0:73";
	int port_fixed	= 8;
	
	//manta
	//char *dest_fixed = "5E:27:46:65:82:AE";
	//int port_fixed	= 24;
	


	struct sockaddr_rc addr = { 0 };
	int status;
	int s;
	char key[key_len +1];
	char *c = (char*) malloc(sizeof(char));
	//int data = 0x01;

	s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
	addr.rc_family = AF_BLUETOOTH;
	//addr.rc_channel = (uint8_t) port;
	//str2ba( dest, &addr.rc_bdaddr );
	if(FIXED){
		addr.rc_channel = (uint8_t) port_fixed;
		str2ba( dest_fixed, &addr.rc_bdaddr );	
	}
	printf("rfcomm_client() establishing connection...\n");
	status = connect(s, (struct sockaddr *)&addr, sizeof(addr));

	// send a message
	if( status == 0 ) {
		status = write(s, "KEY_REQ", 7);
		//status = write(s, "1", 1);
		if( status < 0 ) {
			perror("write error\n");
			close(s);
			return -2;
		}
		printf("rfcomm_client() key_len = %d\n", key_len);
		int i = 0;
		int count = 0;
		while(count < key_len){
            ioctl(s, FIONREAD, &count);
            printf("io: %d\n",count);
            sleep(1);
        }
        printf("count: %d\n",count);
        
		for(i = 0; i < key_len; i++){ 
			status = read(s, c, 1);
			//printf("rfcomm_client() recived: %s, status: %d \n", c, status);
			key[i] = *c; 

		}		
		close(s); 
//  wstaw znak zakonczenia napisu
		key[key_len] = '\0';
		printf("rfcomm_client() recived key: %s, status: %d \n", key, status);

		if( status < 0 ){
			perror("rfcomm_client() read error\n");
			return -3;
		}
        //strcpy(key_rec, key);
		return 0;
	}
	else{
		perror("rfcomm_client(): connect error\n");
		return -1;
	}
}


