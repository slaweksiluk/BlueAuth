//Example 3-3. rfcomm-client.c
//gcc rfcomm_client.c -o rfcomm_client -lbluetooth
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>


int main(int argc, char **argv)
{
	struct sockaddr_rc addr = { 0 };
	int s, status;
	char buf[6] = {0};
	char dest[18] = "8c:71:f8:99:f0:73";
	// allocate a socket
	s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
	// set the connection parameters (who to connect to)
	addr.rc_family = AF_BLUETOOTH;
	addr.rc_channel = (uint8_t) 9;
	str2ba( dest, &addr.rc_bdaddr );
	// connect to server
	status = connect(s, (struct sockaddr *)&addr, sizeof(addr));

	// send a message
	if( status == 0 ) {
		status = write(s, "hello!", 6);
		if( status < 0 ) {
			perror("write error");
			close(s);
			return 0;
		}
		status = read(s, buf, sizeof(buf));
		printf("Recived: %s", buf);
	}	
	if( status < 0 ) perror("read error");
		close(s);
	if(buf[0] == "u"){
		printf("return: 0");
		return 0;
	}
	else return -1;
}

