/* These #defines must be present according to PAM documentation. */
#define PAM_SM_AUTH


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <security/pam_modules.h> //moduły pam
#include <syslog.h> // logi
#include <security/pam_ext.h> // logi
#include <stdbool.h>
#include <openssl/rand.h>
#include "blue_auth.h"

#ifndef PAM_EXTERN
#ifdef PAM_STATIC
#define PAM_EXTERN static
#else
#define PAM_EXTERN extern
#endif
#endif


#ifndef LOG_IDENT
#define LOG_IDENT "pam"
#endif



PAM_EXTERN int
pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char *argv[]){
	const char* pUsername; 
	int retval; //zwracane przez pam
	int status; // zwracane przez moje moudły 
        

        char btaddr[18];
        int  port;
        char mobile_id[50];
        float rssi_threshold;
        float rssi_result;
        char ct[CIPHER_TEXT_LEN_B64];
        unsigned char session_id[SESSION_ID_LEN_BYTES];
        char* session_id_b64;
        int time_out = 60;
	bool dbg = false;
        bool verbose = true;

//  Czy logowac do /var/log/auth.log? 
	int i=0;
	for(i=0; i < argc; i++){
		//pam_syslog(pamh, LOG_INFO, "i = %d\n", i);
		if(strcmp(argv[i], "debug") == 0){
			pam_syslog(pamh, LOG_INFO, "Started...");
			dbg = true;
		}
	}

// czy istnieja pliki konfiguracyjne?
	if(!(file_exists(inifile_users)) || !(file_exists(inifile_mobiles))){
		if(dbg) pam_syslog(pamh, LOG_ERR, "Error: Configuration files not exist\n");
		return PAM_AUTHINFO_UNAVAIL;
	}

	retval =  pam_get_user(pamh, &pUsername, NULL);
	if (retval == PAM_SUCCESS){
		if(dbg) pam_syslog(pamh, LOG_INFO, "pam_get_user() returned user: %s\n", pUsername);
// pobierz dane do polaczenia
		status = get_user_config(&pUsername, btaddr, &port, mobile_id, &rssi_threshold);
		if(status < 0) {
			if(dbg) pam_syslog(pamh, LOG_ERR, "check_user() returned err_code: %d \n", 
                                status);
			return PAM_AUTHINFO_UNAVAIL;
		}
                
//  Wygeneruj session ID
                RAND_pseudo_bytes(session_id, SESSION_ID_LEN_BYTES);
                Base64Encode(session_id, sizeof(unsigned char)*SESSION_ID_LEN_BYTES, &session_id_b64);
                if(dbg) pam_syslog(pamh, LOG_ERR, "Generated session id base 64 encoded: %.12s\n", 
                        session_id_b64);

//  polaczenie
		status = rfcomm_client(btaddr, port, ct, session_id_b64, time_out, &rssi_result);
		if(status < 0) {
			if(dbg) pam_syslog(pamh, LOG_ERR, "rfcomm_client() returned err_code: %d \n", status);
			return PAM_SERVICE_ERR;
		}
                if(dbg) pam_syslog(pamh, LOG_INFO, "Received cipher text: %s\n", ct);                
                if(dbg) 
                    pam_syslog(pamh, LOG_INFO, "Meausured rssi: %f\n", rssi_result);
                if(verbose) fprintf(stderr, "Meausured rssi: %f\n", rssi_result);
 
//  Oborobka szyfrogramu
                char priv_key_dir[256];
                snprintf(priv_key_dir, sizeof priv_key_dir, "%s%s%s", PRIV_KEYS_PATH, mobile_id, ".pem");
                char hash_dir[256];
                snprintf(hash_dir, sizeof hash_dir, "%s%s", HASHES_PATH, mobile_id);
                printf("private key dir: %s\n", priv_key_dir);
                printf("hash dir: %s\n", hash_dir);
                status = verify_ciphertext(ct, priv_key_dir, hash_dir, session_id);
                if(status < 0){
                    if(dbg) pam_syslog(pamh, LOG_ERR, "verify_ciphertext() returned err_code: %d \n", status);
                    return PAM_SERVICE_ERR;
                }
                
//  Uwierzytelnienie udane!
		if(dbg) pam_syslog(pamh, LOG_INFO, "Authentication success for user: %s\n", pUsername);
		return PAM_SUCCESS;
	
	}
	else {
		if(dbg) pam_syslog(pamh, LOG_ERR, "pam_get_user() ERROR: %s\n", pam_strerror(pamh, retval));
		return PAM_USER_UNKNOWN;
	}


}

PAM_EXTERN int
pam_sm_setcred(pam_handle_t *pamh, int flags,
	int argc, const char *argv[])
{

	return (PAM_IGNORE);
}

PAM_EXTERN int
pam_sm_acct_mgmt(pam_handle_t *pamh, int flags,
	int argc, const char *argv[])
{

	return (PAM_IGNORE);
}

PAM_EXTERN int
pam_sm_open_session(pam_handle_t *pamh, int flags,
	int argc, const char *argv[])
{

	return (PAM_IGNORE);
}

PAM_EXTERN int
pam_sm_close_session(pam_handle_t *pamh, int flags,
	int argc, const char *argv[])
{

	return (PAM_IGNORE);
}

PAM_EXTERN int
pam_sm_chauthtok(pam_handle_t *pamh, int flags,
	int argc, const char *argv[])
{

	return (PAM_IGNORE);
}



#ifdef PAM_STATIC

struct pam_module _pam_blueAuth_modstruct = {
  "pam_blueAuth",
  pam_sm_authenticate,
  pam_sm_setcred,
  pam_sm_acct_mgmt,
  pam_sm_open_session,
  pam_sm_close_session,
  pam_sm_chauthtok
};

#endif
