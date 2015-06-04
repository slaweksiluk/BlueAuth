#include <stdio.h>
#include <string.h>
#include "blue_auth.h"
#include "../minini/minIni.h"
#include <unistd.h>
#include <stdbool.h>

#define sizearray(a)  (sizeof(a) / sizeof((a)[0]))

bool file_exists(const char *f){
	if( access( f, F_OK ) != -1 ) {
		return true;
	} else {
		return false;
	}
}

int get_key_len(const char **user){
    char section[50]; //nazwa sekcji
    char mobile[50]; //nazwa telefonu dla uzytkownika

	int key_len; // dlogosc klucza z konfigu
    int  s; //iterator sekcji
    int  b; // enabled, true/false

//  Szukaj uzytkownikow
	for (s = 0; ini_getsection(s, section, sizearray(section), inifile_users) > 0; s++) {
		printf("get_key_len() [%s] user: %s\n", section, *user);
		if (!strcmp(*user, section)){
			printf("user cfg exists\n");
			b = ini_getbool(section, "enabled", 0, inifile_users);
			if(b == 1){
			//pobierz nazwe telfonu przypisanego do uzytkownika
				ini_gets(section, "mobile", "0", mobile, sizearray(mobile), inifile_users);
			//  pobierz dlugosc klucza i klucz
				key_len = ini_getl(mobile, "key_len", 1, inifile_mobiles);
				printf("get_key_len(): key length: %d\n", key_len);
				return key_len;
			}
		}
	} /* for */
	return -1;
}
	

// Wejscie:
//      user - login
// Wyjscia
//      btaddr
//      port
//      mobile_id 
//      rssi
int get_user_data(const char **user, char* btaddr, int *port, char *mobile_id,
        float *rssi){
    char section[50]; //nazwa sekcji
    char mobile[50]; //nazwa telefonu dla uzytkownika
    char addr[18];

    int  s; //iterator sekcji
    int  b; // enabled, true/false
    int  p; // port
    float min_rssi; // rssi
    //char user_login[] = *user;

      for (s = 0; ini_getsection(s, section, sizearray(section), inifile_users) > 0; s++) {
        //printf("    [%s] user: %s\n", section, *user);

        if (!strcmp(*user, section)){
            printf("user cfg exists\n");

            b = ini_getbool(section, "enabled", 0, inifile_users);
            if(b == 1){
//pobierz nazwe telfonu przypisanego do uzytkownika
                ini_gets(section, "mobile", "0", mobile, sizearray(mobile), inifile_users);
//pobierrz btaddr i port telefonu o ww nazwie
                ini_gets(mobile, "btaddr", "999", addr, sizearray(addr),
                 inifile_mobiles);
                p = ini_getl(mobile, "port", 999, inifile_mobiles);
//  pobierz wartosc progu RSSI
                min_rssi = ini_getf(mobile, "min_rssi", "-5.0", inifile_users);
                
                printf("user %s enabled with mobile: %s\n", *user, mobile);
                printf("mobile btaddr: %s, port: %d\n", addr, p);
                printf("rssi threshold: %f\n", min_rssi);
                //printf("check_user(): key length: %d, key: %s\n", key_len, k);

//zapisz btaddr, port i klucz do adresow z argumetu
                strcpy(btaddr,addr);
                strcpy(mobile_id, mobile);
                *port = p;
                *rssi = min_rssi;
                return 0;
            }

            //for (k = 0; ini_getkey(section, k, str, sizearray(str), inifile) > 0; k++) {
            //  printf("\t%s\n", str);
            //} /* for */
        }
      } /* for */
return E_INI_FILE;
}



