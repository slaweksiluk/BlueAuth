// #include <stdio.h>
//#include <stdlib.h>
// #include <unistd.h>
// gcc -fPIC -DPIC -shared -rdynamic -lbluetooth -o pam.so pam.c 
//gcc -fPIC -c pam.c
//gcc -shared -o pam.so pam.o -lpam -lbluetooth
#define PAM_SM_AUTH
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/socket.h>
#include <syslog.h>
//#include "blue_auth.h"



#ifdef HAVE_CONFIG_H
# include <config.h>
#endif
#include <sys/param.h>
//#include <security/pam_modules.h>
#include <security/pam_ext.h>
#define DEFAULT_NOLOGIN_PATH "/etc/nologin"
PAM_EXTERN int
pam_sm_authenticate(pam_handle_t *pamh, int flags, 	int argc, const char *argv[]){

// log 
pam_syslog(pamh, LOG_INFO, "moje logiii");

          struct passwd *pw = NULL, pw_s;
        const char *user = NULL;
        char btaddr[50];
        int port; 

          char buffer[1024], checkfile[1024];
          int pgu_ret, gpn_ret, snp_ret, a_ret;



          pgu_ret = pam_get_user(pamh, &user, NULL);



fprintf(stderr, "%s", user);
fprintf(stderr, "\n");

          if (pgu_ret != PAM_SUCCESS || user == NULL) {
                  return(PAM_IGNORE);
          }

/*          gpn_ret = getpwnam_r(user, &pw_s, buffer, sizeof(buffer), &pw);*/
/*          if (gpn_ret != 0 || pw == NULL || pw->pw_dir == NULL || pw->pw_dir[0] != '/') {*/
/*                  return(PAM_IGNORE);*/
/*          }*/

          //snp_ret = snprintf(checkfile, sizeof(checkfile), "%s/.pam_blue_auth", pw->pw_dir);
          //if (snp_ret >= sizeof(checkfile)) {
          //        return(PAM_IGNORE);
          //}

            a_ret = check_user_rules(&user, &btaddr, &port);

          //a_ret = access(checkfile, F_OK);
          if (a_ret == 0) {
			fprintf(stderr, "The user's file exists\n");
			if(rfcomm_client(btaddr, port)==0){
			//if(1==1){
				fprintf(stderr, "return authentication success\n");
                 return(PAM_SUCCESS);
			}
			else{
				fprintf(stderr, "return authentication success\n");
				return(PAM_AUTH_ERR);
			}
		}
		 else{
fprintf(stderr, "The user's doesn't file exist, return authentication PAM_IGNORE\n");
                  return(PAM_AUTH_ERR);
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


