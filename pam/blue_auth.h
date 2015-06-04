#ifndef BLUE_AUTH_H_INCLUDED
#define BLUE_AUTH_H_INCLUDED
#include <stdbool.h>
#include <stdio.h>

//  ini_config.c
int get_user_config(const char **user, char* btaddr, int *port, char *mobile_id,
        float *rssi);
bool file_exists(const char *f);
int get_key_len(const char **user);

//  bt_connection.c
int rfcomm_client(const char *dest, const int port, char *rec_ct, 
        const char* session_id, const int max_delay, float* rssi_value);

//  crypto_engine.c 
unsigned char* decrypt(const char *ct, const char* priv_key_dir);
unsigned char* calc_hash(unsigned char* mess1, int mess1_len);
int verify_ciphertext(const char* ct, const char* priv_key_dir, 
        const char* key_id_hash_dir, const char* session_id_b64);

//  base64.c
int Base64Decode(const char* b64message, unsigned char** buffer, size_t* length);
int Base64Encode(const unsigned char* buffer, size_t length, char** b64text);

// constants
static const int CIPHER_TEXT_LEN_BYTES = 128;
static const int CIPHER_TEXT_LEN_B64 = 172;
static const int SESSION_ID_LEN_B64 = 12;
static const int SESSION_ID_LEN_BYTES = 8;
static const int HASH_LEN_B64 = 88;
static const int HASH_LEN_BYTES = 64;
static const int KEY_ID_LEN_BYTES = 32;
static const int MSG_LEN_BYTES = 8;

static const char inifile_users[] = "../etc/BlueAuth/users.conf";
static const char inifile_mobiles[] = "../etc/BlueAuth/mobiles.conf";
//const static int MSG_LEN = 8;
const static bool FIXED = true;

//  Error codes
//typedef enum {
//    BLUE_ERROR,
//    BLUE_NOT_CONNECTED
//} ECODES;

typedef enum _config_error
{
    E_SUCCESS = 0,
    E_HCI_BT_ERROR  = -1,
    E_HCI_BT_NOT_CONNECTED  = -2,
    E_BT_SEND_ERR = -3,
    E_BT_RECEIVE_ERR = -4,
    E_THREAD_CREATE_ERR = -5,
    E_UNKNOWN_CONTROL_MSG = -6,
    E_USR_RES_TIME_OUT = -7,
    E_RECEIVED_AUTH_DENY = -8,
    E_BT_CONNECT_ERR = -9,
    E_PRIV_KEY_FILE = -10,
    E_DECRYPT_ERR = -11,
    E_SESSION_ID_DIFF = -12,
    E_HASH_DIFF = -13,
    E_HASH_FILE = -14,
    E_INI_USR_NOT_FOUND = -15,
    E_INI_USR_DISABLED = -16,
    E_INI_FILES_NOT_EXIST = -17,
} error_t;

/* type to provide in your API */
//typedef _config_error error_t;

/* use this to provide a perror style method to help consumers out */
static struct _errordesc {
    int  code;
    char *message;
} errordesc[] = {
    { E_SUCCESS, "No error, exiting..." },
    { E_HCI_BT_ERROR, "HCI: Generic BT HCI error" },
    { E_HCI_BT_NOT_CONNECTED, "HCI: BT device not connected" },
    { E_BT_SEND_ERR, "Unable to send BT data"},
    { E_BT_RECEIVE_ERR, "Unable to receive BT data"},
    { E_THREAD_CREATE_ERR, "Unable to create thread"},
    { E_UNKNOWN_CONTROL_MSG, "Unknown control msg recived from mobile"},
    { E_USR_RES_TIME_OUT, "Mobile auth confirmation time exceed"},
    { E_RECEIVED_AUTH_DENY, "Auth deny message received form mobile"},
    { E_BT_CONNECT_ERR, "Unable to connect to mobile"},
    { E_PRIV_KEY_FILE, "Unable to read private key pem file"},
    { E_DECRYPT_ERR, "Unable to get possible decrypt length"},
    { E_SESSION_ID_DIFF, "Received session ID diffrent from sent session ID"},
    { E_HASH_DIFF, "Received key ID hash diffrent from hash read from conf"},
    { E_HASH_FILE, "Unable to open hash file"},
    { E_INI_USR_NOT_FOUND, "User configuration data not found"}, 
    { E_INI_USR_DISABLED, "User enabled flag not set"}, 
    { E_INI_FILES_NOT_EXIST, "Configuration files not exist"}, 
};

//  Printing errors
static inline void print_error(int ecode){
    printf("Error code: %d. %s\n", ecode, errordesc[ecode*(-1)].message);
}


#endif
