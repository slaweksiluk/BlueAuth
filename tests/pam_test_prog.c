#include <security/pam_appl.h>
#include <security/pam_misc.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

const struct pam_conv conv = {
	misc_conv,
	NULL
};

int main(int argc, char *argv[]) {
	pam_handle_t* pamh = NULL;
	int retval;
	const char* user;

	user = getlogin();
	printf("Pam test prog: Process called by: %s\n", user);

	retval = pam_start("pam-test-prog", user, &conv, &pamh);
	if (retval == PAM_SUCCESS) {
		printf("Pam test prog: app started\n");
		//retval = pam_authenticate(pamh, 0);
		retval = pam_authenticate(pamh, 0);
	}
	else printf("Pam_test_prog: function pam_start() ERROR: %s\n", 
	    pam_strerror(pamh, retval));

	if (retval == PAM_SUCCESS) {
		printf("Pam test prog: pam_authenticate - PAM_SUCCESS.\n");	
	}
	else printf("Pam_test_prog: function pam_authenticate() ERROR: %s\n", 
	    pam_strerror(pamh, retval));

	if (pam_end(pamh, retval) != PAM_SUCCESS) {
		pamh = NULL;
		printf("Pam test prog:: failed to release authenticator\n");
		exit(1);
	}
	return retval == PAM_SUCCESS ? 0 : 1;
}
