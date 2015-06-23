//gcc -Wall -o conn_test conn_test.c -pthread -lbluetooth
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/rfcomm.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include "blue_auth.h"


//  head -c 8 /dev/urandom | base64
// This is not really thread-safe
// bool error;

typedef struct {
    float rssi;
    const char *addr;
    bool supported;
    bool stop;
} rssi_t;
    
    
int findConn(int s, int dev_id, long arg) {
	struct hci_conn_list_req *cl;
	struct hci_conn_info *ci;
	
	if ((cl = malloc(HCI_MAX_DEV * sizeof(*ci) + sizeof(*cl))) == NULL) {
		perror("malloc");
//		error = true;
		return 0;
	}
	cl->dev_id = dev_id;
	cl->conn_num = HCI_MAX_DEV;
	ci = cl->conn_info;

	if (ioctl(s, HCIGETCONNLIST, (void *)cl)) {
		perror("Could not get connection list");
//		error = true;
		free(cl);
		return 0;
	}

	int i;
	for (i = 0; i < cl->conn_num; i++, ci++) {
		if (!bacmp((bdaddr_t *)arg, &ci->bdaddr)) {
			free(cl);
			return 1;
		}
	}

	free(cl);
	return 0;
}


void *measure_rssi(void *parm){
    fprintf(stderr, "measure_rssi()   started...\n");

    int rc;
    rssi_t *rssi_st = parm;
    int i=0;
    float rssi_sum=0;
  

    const char *address = rssi_st->addr;
//  Domyslnie
    rssi_st->supported = false;
    rssi_st->rssi = -1;
//  Skopiowane z hci_rssi()
	struct hci_conn_info_req *cr;
	int8_t rssi;
	int dd, dev_id;
	
	bdaddr_t bdaddr;
	str2ba(address, &bdaddr);
        
        fprintf(stderr, "measure_rssi()   calling hci for ...\n");
	//error = false;
	dev_id = hci_for_each_dev(HCI_UP, findConn, (long)&bdaddr);
        fprintf(stderr, "measure_rssi()   dev id is %d\n", dev_id);
	if (dev_id < 0) {
//        if (error) {
//			rc = E_HCI_BT_ERROR;
//		    pthread_exit(NULL);
//		}
//		else {
//		    rc = E_HCI_BT_NOT_CONNECTED;
//	        pthread_exit(NULL);
//		}
	}


        
        
	dd = hci_open_dev(dev_id);
        fprintf(stderr, "measure_rssi()   dd is %d\n", dd);
	if (dd < 0) {
		perror("Could not open HCI device");
		rc = E_HCI_BT_ERROR;
	    pthread_exit(NULL);
	}

	if ((cr = malloc(sizeof(struct hci_conn_info_req) + sizeof(struct hci_conn_info))) == NULL) {
		perror("malloc");
		hci_close_dev(dd);
		rc = E_HCI_BT_ERROR;
	    pthread_exit(NULL);
	}

	bacpy(&cr->bdaddr, &bdaddr);
	cr->type = ACL_LINK;
	if (ioctl(dd, HCIGETCONNINFO, (unsigned long)cr) < 0) {
		perror("Could not get connection info");
		free(cr);
		hci_close_dev(dd);
	    rc = E_HCI_BT_ERROR;
        pthread_exit(NULL);
	}
//  Do zapisu pomiarow...
    //FILE *f = fopen("rssi_samples.dat", "wt");
    while(true){
        rc = hci_read_rssi(dd, htobs(cr->conn_info->handle), &rssi, 1000);
    	if(rc < 0) break;
        rssi_sum += rssi;
	    //fprintf(f, "%d\n", rssi);
        i++;	
    }
    //fclose(f);

    
    if((rc == E_HCI_BT_NOT_CONNECTED) || (rssi_st->stop)){
        //perror("HCI_ERROR");
        //print_error(rc);
        rssi_st->supported = true;
        rssi_st->rssi = (rssi_sum/i);
        //printf("Mean RSSI value: %f\n, with %d iters\n", rssi_st->rssi, i);

    } else{
        //perror("HCI_ERROR");
        //print_error(rc);
        rssi_st->supported = false;
        rssi_st->rssi = -1;
    }
    
	free(cr);
	hci_close_dev(dd);    
    pthread_exit(NULL);
}
  

int send_msg(const char *msg, int s, int msg_len){
    int status;
    printf("send_msg() sent: %.8s\n", msg);
	status = write(s, msg, msg_len);
	if( status < 0 ) {
		close(s);
		return E_BT_SEND_ERR;
	}
	return 0;
}

int get_msg(char *msg, int s, int msg_len){
    int status;
    int i;
	char *c = (char*) malloc(sizeof(char));
	
    for(i = 0; i < msg_len; i++){ 
	    status = read(s, c, 1);
	    //printf("rfcomm_client() recived: %s, status: %d \n", c, status);
	    msg[i] = *c; 
    }	
	if( status < 0 ) {
		close(s);
		return E_BT_RECEIVE_ERR;
	}
    //  Wstaw koniec stringa
    msg[msg_len] = '\0';
    printf("get_msg() received: %.8s\n", msg);
	return 0;
}

int rfcomm_client(const char *dest, const int port, char *rec_ct, 
        const char* session_id, const int max_delay, float* rssi_value){
    const static bool CHECK_RSSI = true;

    bool CONFIRM;
    
    // return code
    int rc;
    
    // ?
    int count = 0;
    
    // RSSI
    pthread_t rssi_th;
    rssi_t rssi_st;
    rssi_st.addr = dest;
    rssi_st.stop = false;

    // ?
    int seconds;
    struct timeval start, end;

    struct sockaddr_rc addr = { 0 };
    int status;
    int s;
    char ct[CIPHER_TEXT_LEN_B64];
    char cnt_msg[MSG_LEN_BYTES];
	

    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    addr.rc_family = AF_BLUETOOTH;

    addr.rc_channel = (uint8_t) port;
    str2ba( dest, &addr.rc_bdaddr );	

    printf("Establishing connection...\n");
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));
    fprintf(stderr, "rfcomm_client()   socket is %d\n", s);

//  Utworzono polaczanie. Mozna rozpoczac pomiar mocy sygnalu,
//  jesli ma byc on przeprowadzony... trzeba utworzyc do tego watek
//  argumet rssi to aktualna wartosc odczytu
    if(CHECK_RSSI){
        fprintf(stderr, "rfcomm_client() creating rssi thread...\n");
        rc = pthread_create(&rssi_th, NULL, measure_rssi, &rssi_st);
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(E_THREAD_CREATE_ERR);
        }
    }

//  Wyslij wiadomosc sterujaca
        if( status == 0 ) {
            rc = send_msg("AUTH_REQ", s, MSG_LEN_BYTES);
                if(rc<0){
                    print_error(rc);
                    return rc;
                }
//  Wyslij token sesji
            rc = send_msg(session_id, s, SESSION_ID_LEN_B64);
                if(rc<0){
                    print_error(rc);
                    return rc;
                }

//  Odbierz wiadomosc conf/non_conf
        rc = get_msg(cnt_msg, s, MSG_LEN_BYTES);
        if(rc < 0){
            print_error(rc);
            return rc;
        }   	


        if(!strcmp(cnt_msg,"NON_CONF")){
            CONFIRM = false;
        } else if(!strcmp(cnt_msg,"USR_CONF")){
            CONFIRM = true;
//  Odmierzaj czas
            gettimeofday(&start, NULL); 

        } else{
            print_error(E_UNKNOWN_CONTROL_MSG);
            return E_UNKNOWN_CONTROL_MSG;
        }

        while(!count){
            if(CONFIRM){
                sleep(1);
                //  Sprawdz czy nie przekoroczny czas
                gettimeofday(&end, NULL); 
                seconds  = (end.tv_sec-start.tv_sec);         
                //printf("rfcomm_client(): current delay is %d\n", seconds); 
                if(seconds > max_delay){
                    //  Przekroczony,  wyslij time_out   
                    if((rc = send_msg("TIME_OUT",s, MSG_LEN_BYTES))){
                        print_error(rc);
                        close(s);
                        return rc;
                    }
                    printf("Reached max delay time, exiting...\n");
                    rssi_st.stop = true;
                    close(s);
                    print_error(E_USR_RES_TIME_OUT);
                    pthread_join(rssi_th, NULL);
                    return E_USR_RES_TIME_OUT;
                }
            }
        //  Sprawdz czy jest cos do odebrania  
        ioctl(s, FIONREAD, &count);
        }

                //printf("rfcomm_client() something to receive... \n");  
        //  Odbierz rezultat
        rc = get_msg(cnt_msg, s, MSG_LEN_BYTES);
        if(rc < 0){
            printf("\nget_msg err\n");
            print_error(rc);
            return rc; 
        }

        if(!strcmp(cnt_msg, "AUT_PERM")){

            //  Odbierz szyfrogram
            if((rc = get_msg(ct, s, CIPHER_TEXT_LEN_B64))){
                print_error(rc);
                return rc;
            }	
            rssi_st.stop = true;
            close(s);
            pthread_join(rssi_th, NULL);

        } else if(!strcmp(cnt_msg, "AUT_DENY")){
            print_error(E_RECEIVED_AUTH_DENY);
            close(s);
            return E_RECEIVED_AUTH_DENY;
        } else{
            print_error(E_UNKNOWN_CONTROL_MSG);
            close(s);
            return E_UNKNOWN_CONTROL_MSG;
        }

//  Komunikacja udana
        //print_error(E_BT_COMM_SUCCESS);
                // Skopiuj odebrany ct do argumentu
                strcpy(rec_ct, ct);
                // Ustaw w argumenice otrzymana wartosc rssi
                *rssi_value = rssi_st.rssi;
                // Zwroc sukces!
                return E_SUCCESS;
        } else{
                print_error(E_BT_CONNECT_ERR);
                return E_BT_CONNECT_ERR;
        }
}
