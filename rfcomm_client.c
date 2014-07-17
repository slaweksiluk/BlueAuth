//Example 3-3. rfcomm-client.c
//gcc rfcomm_client.c -o rfcomm_client  -lbluetooth
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>



/*int main(int argc, char **argv)*/
int main()
{

  char *dest = NULL;
  char *port = NULL;
  char *line = NULL;
  size_t len = 0;

  ssize_t read_s;

  FILE *fp;
   fp = fopen("/etc/blueAuth.conf", "r");
   if (fp == NULL) printf("file not open\n");

	read_s = getline(&line, &len, fp); //komentarz
	read_s = getline(&line, &len, fp); //UUID
	read_s = getline(&dest, &len, fp); //btaddr

int dest_len;					//removes newline character
dest_len = strlen(dest);
if( dest[dest_len-1] == '\n' )
    dest[dest_len-1] = 0;
				printf("%s\n", dest);

	read_s = getline(&port, &len, fp); //port
int port_int = atoi( port );
               printf("%d", port_int);


//	fprintf(stderr,"Btaddr: %s\n", dest);
	//fprintf(stderr,"Btaddr: %s\n", &dest);

	

	struct sockaddr_rc addr = { 0 };
	int status;
	int s;
	char buf[6] = {0};
	char *compare;
	compare = "u";
	//char dest[18] = "8c:71:f8:99:f0:73";
	printf("allocate a socket\n");
	s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
	// set the connection parameters (who to connect to)
	addr.rc_family = AF_BLUETOOTH;
	addr.rc_channel = (uint8_t) port_int;
	str2ba( dest, &addr.rc_bdaddr );
	printf("connect to server\n");
	status = connect(s, (struct sockaddr *)&addr, sizeof(addr));

	// send a message
	if( status == 0 ) {
		status = write(s, "hello!", 6);
		if( status < 0 ) {
			perror("write error\n");
			close(s);
			return -1;
		}
		status = read(s, buf, sizeof(buf));
		printf("Recived: %s\n", buf);
		close(s);
		if( status < 0 ){
			perror("read error\n");
			close(s);
			return -1;
		}
		if(buf[0] == *compare){
			printf("return: 0\n");
			return 0;
		}
	}	
	else{
		perror("connect error\n");
		return -1;
	}
	printf("return: 0\n");
	return 0;
}

