#include <security/pam_modules.h> //moduły pam
#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include<unistd.h>
#include<stdbool.h>
#include"xidle.h"


PAM_EXTERN int
pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char *argv[]){
    //const static int TIME_DIFF_MSECONDS = 0.5;
    const static int SAMPLE_PERIOD = 5;
    const static int DEBOUNCES = 2;
    det_user_interrupt(SAMPLE_PERIOD, DEBOUNCES, true);
    return PAM_SUCCESS;
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


