#define PAM_SM_AUTH
#define PAM_SM_ACCOUNT
#define PAM_SM_SESSION
#define PAM_SM_PASSWORD
#include <security/pam_modules.h>
#include <security/_pam_macros.h>

#include <syslog.h> // logi
#include <security/pam_ext.h> // logi
#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include<unistd.h>
#include<stdbool.h>
#include <inttypes.h>
#include"xidle.h"



PAM_EXTERN int
pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char *argv[]){
    // Domyslnie
    int sample_period  = 5;
    int debounces = 2;
    int pre_delay = 500;

    // TO DO - zrobic argumenty klucz=wartosc i wykrywanie blebych danych
    // Trzy pierwsze argumenty to parametry czasowe odblkowywania. Sa obowiazkowe,
    //      trzeba sprawdzic czy sa podane.
    if(argc < 2){
        pam_syslog(pamh, LOG_ERR, "Obligatory arguments not passed, setting def values");
    } else{
        pam_syslog(pamh, LOG_INFO, "Setting params from arguments...");
        //      1. sample_period - okres pomiedzy pomiarami[ms] nr0
        sample_period = strtoimax(argv[0], NULL, 10);
        //      2. debounces - liczba koniecnzych okresow z ruchem nr1
        debounces = strtoimax(argv[1], NULL, 10);
        //      3. pre_delay - czas opoznienia przed rozpoczeciem probkowania[ms] nr2
        pre_delay = strtoimax(argv[2], NULL, 10);
    }

    
    // dalsze argumenty to: debug i verbose
    // Flaga logowania do /var/log/auth.log
    bool dbg = false;
    // Flaga pisania do stderr
    bool v = false;
    int i=0;
    for(i=0; i < argc; i++){
        if(strcmp(argv[i], "debug") == 0){
            pam_syslog(pamh, LOG_INFO, "Started...");
            dbg = true;
        }
        if(strcmp(argv[i], "verbose") == 0){
            pam_syslog(pamh, LOG_INFO, "Print to stderr enabled");
            v = true;
        }
    }

    usleep(pre_delay * 1000);
    if(det_user_interrupt(sample_period, debounces, v) == -1){
        pam_syslog(pamh, LOG_ERR, "ERROR display NULL");
        return PAM_IGNORE;
    }
    if(dbg) pam_syslog(pamh, LOG_ERR, "Returning PAM_SUCCESS");
    return PAM_SUCCESS;
}

PAM_EXTERN int
pam_sm_setcred(pam_handle_t *pamh, int flags,
	int argc, const char *argv[])
{

  return PAM_SUCCESS;
}

PAM_EXTERN int
pam_sm_acct_mgmt(pam_handle_t *pamh, int flags,
	int argc, const char *argv[])
{

  return PAM_SUCCESS;
}

PAM_EXTERN int
pam_sm_open_session(pam_handle_t *pamh, int flags,
	int argc, const char *argv[])
{

  return PAM_SUCCESS;
}

PAM_EXTERN int
pam_sm_close_session(pam_handle_t *pamh, int flags,
	int argc, const char *argv[])
{

  return PAM_SUCCESS;
}

PAM_EXTERN int
pam_sm_chauthtok(pam_handle_t *pamh, int flags,
	int argc, const char *argv[])
{

  return PAM_SUCCESS;
}

#ifdef PAM_STATIC

/* static module data */

struct pam_module _pam_permit_modstruct = {
  "pam_permit",
  pam_sm_authenticate,
  pam_sm_setcred,
  pam_sm_acct_mgmt,
  pam_sm_open_session,
  pam_sm_close_session,
  pam_sm_chauthtok
};

#endif


