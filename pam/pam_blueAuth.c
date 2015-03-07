#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <security/pam_modules.h> //moduły pam
#include <syslog.h> // logi
#include <security/pam_ext.h> // logi
#include "blue_auth.h"



PAM_EXTERN int
pam_sm_authenticate(pam_handle_t *pamh, int flags, 	int argc, const char *argv[]){
	pam_syslog(pamh, LOG_INFO, "started");

	const char* pUsername; 
	int retval; //zwracane przez pam
	int status; // zwracane przez moje moudły 

    char btaddr[18];
    int  port;

	retval = pam_get_user(pamh, &pUsername, NULL);
	if (retval == PAM_SUCCESS){
		pam_syslog(pamh, LOG_INFO, "pam_get_user() returned user: %s\n", pUsername);
		int key_len = get_key_len(&pUsername);
		if(key_len < 0) {
			pam_syslog(pamh, LOG_INFO, "get_key_len() returned err_code: %d \n", key_len);
			return PAM_AUTHINFO_UNAVAIL;
		}
// pobierz dane do polaczenia
		char key_cfg[key_len];
		status = check_user(&pUsername, btaddr, &port, key_cfg);
		if(status < 0) {
			pam_syslog(pamh, LOG_INFO, "check_user() returned err_code: %d \n", status);
			return PAM_AUTHINFO_UNAVAIL;
		}
//  polaczenie
		char key_rec[key_len];
		status = rfcomm_client(btaddr,port, key_len, key_rec);
		if(status < 0) {
			pam_syslog(pamh, LOG_INFO, "rfcomm_client() returned err_code: %d \n", status);
			return PAM_SERVICE_ERR;
		}
//  Uwierzytelnienie udane!
		pam_syslog(pamh, LOG_INFO, "autchenticate success for user: %s\n", pUsername);
		return PAM_SUCCESS;
	
	}
	else {
		pam_syslog(pamh, LOG_INFO, "pam_get_user() ERROR: %s\n", pam_strerror(pamh, retval));
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

