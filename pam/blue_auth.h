#ifndef BLUE_AUTH_H_INCLUDED
#define BLUE_AUTH_H_INCLUDED
#include <stdbool.h>
//int check_user(char **user, char* btaddr, int *port);
int check_user(const char **user, char* btaddr, int *port, char *key);
//int rfcomm_client(const char *dest, const int port);
int rfcomm_client(const char *dest, const int port, const int key_len, char *key_rec);
int get_key_len(const char **user);
bool file_exists(const char *f);

//  crypto_engine.c 
unsigned char* decrypt(char *ct, char* priv_key_dir);
unsigned char* calc_hash(unsigned char* mess1, int mess1_len);

//  base64.c
int Base64Decode(char* b64message, unsigned char** buffer, size_t* length);
int Base64Encode(const unsigned char* buffer, size_t length, char** b64text);

// constants
static const int key_len = 1024;
static const int session_id_len = 8;
static const int key_id_len = 8;
static const char inifile_users[] = "/etc/blueAuth/users.conf";
static const char inifile_mobiles[] = "/etc/blueAuth/mobiles.conf";
//const static int MSG_LEN = 8;
const static bool FIXED = true;


//  Error codes
//typedef enum {
//    BLUE_ERROR,
//    BLUE_NOT_CONNECTED
//} ECODES;

typedef enum _config_error
{
    //E_SUCCESS = 0,
    E_INVALID_INPUT = -1,
    E_FILE_NOT_FOUND = -2, /* consider some way of returning the OS error too */
    E_HCI_BT_ERROR  = -3,
    E_HCI_BT_NOT_CONNECTED  = -4,
    E_BT_SEND_ERR = -5,
    E_BT_RECEIVE_ERR = -6,
    E_THREAD_CREATE_ERR = -7,
    E_UNKNOWN_CONTROL_MSG = -8,
    E_USR_RES_TIME_OUT = -9,
    E_RECEIVED_AUTH_DENY = -10,
    E_BT_COMM_SUCCESS = 0,
    E_BT_CONNECT_ERR = -11,
    

//  Crypto errors off 20
    E_PRIV_KEY_FILE = -20,
    E_DECRYPT_ERR = -21,
    
} error_t;

/* type to provide in your API */
//typedef _config_error error_t;

/* use this to provide a perror style method to help consumers out */
static struct _errordesc {
    int  code;
    char *message;
} errordesc[] = {
    { E_BT_COMM_SUCCESS, "Successfull session, passing rssi value & key to pam" },
    { E_INVALID_INPUT, "Invalid input" },
    { E_FILE_NOT_FOUND, "File not found" },
    { E_HCI_BT_ERROR, "HCI: Generic BT HCI error" },
    { E_HCI_BT_NOT_CONNECTED, "HCI: BT device not connected" },
    { E_BT_SEND_ERR, "Unable to send BT data"},
    { E_BT_RECEIVE_ERR, "Unable to receive BT data"},
    { E_THREAD_CREATE_ERR, "Unable to create thread"},
    { E_UNKNOWN_CONTROL_MSG, "Unknown control msg recived from mobile"},
    { E_USR_RES_TIME_OUT, "Mobile auth confirmation time exceed"},
    { E_RECEIVED_AUTH_DENY, "Auth deny message received form mobile"},
    { E_BT_CONNECT_ERR, "Unable to connect to mobile"},
//  Crypto errors
    { E_PRIV_KEY_FILE, "Unable to read private key pem file"},
    { E_DECRYPT_ERR, "Unable to get possible decrypt length"},
};

//  Printing errors
static inline void print_error(int ecode){
    printf("Error code: %d. %s\n", ecode, errordesc[ecode*(-1)].message);
}


#endif
