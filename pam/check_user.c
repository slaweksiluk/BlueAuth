#include <stdio.h>
#include <string.h>
#include "blue_auth.h"
#include "../minini/minIni.h"

#define sizearray(a)  (sizeof(a) / sizeof((a)[0]))

int get_key_len(const char **user){
    char section[50]; //nazwa sekcji
    char mobile[50]; //nazwa telefonu dla uzytkownika

	int key_len; // dlogosc klucza z konfigu
    int  s; //iterator sekcji
    int  b; // enabled, true/false

      for (s = 0; ini_getsection(s, section, sizearray(section), inifile) > 0; s++) {
        printf("    [%s] user: %s\n", section, *user);
        if (!strcmp(*user, section)){
            printf("user cfg exists\n");
            b = ini_getbool(section, "enabled", 0, inifile);
            if(b == 1){
//pobierz nazwe telfonu przypisanego do uzytkownika
                ini_gets(section, "mobile", "0", mobile, sizearray(mobile), inifile);
//  pobierz dlugosc klucza i klucz
                key_len = ini_getl(mobile, "key_len", 1, inifile_mobiles);
                printf("get_key_len(): key length: %d\n", key_len);
                return key_len;
            }
        }
      } /* for */
return -1;
}
	

int check_user(const char **user, char* btaddr, int *port, char *key){

    char section[50]; //nazwa sekcji
    char mobile[50]; //nazwa telefonu dla uzytkownika
    char addr[18];

	int key_len; // dlogosc klucza z konfigu
    int  s; //iterator sekcji
    int  b; // enabled, true/false
    int  p;
    //char user_login[] = *user;

      for (s = 0; ini_getsection(s, section, sizearray(section), inifile) > 0; s++) {
        //printf("    [%s] user: %s\n", section, *user);

        if (!strcmp(*user, section)){
            printf("user cfg exists\n");

            b = ini_getbool(section, "enabled", 0, inifile);
            if(b == 1){
//pobierz nazwe telfonu przypisanego do uzytkownika
                ini_gets(section, "mobile", "0", mobile, sizearray(mobile), inifile);
//pobierrz btaddr i port telefonu o ww nazwie
                ini_gets(mobile, "btaddr", "999", addr, sizearray(addr),
                 inifile_mobiles);
                p = ini_getl(mobile, "port", 999, inifile_mobiles);
//  pobierz  klucz
                key_len = ini_getl(mobile, "key_len", 1, inifile_mobiles);
				char k[key_len]; 
                ini_gets(mobile, "key", "key_no_set", k, sizearray(k),
                 inifile_mobiles);		

                printf("check_user(): user %s enabled with mobile: %s\n", *user, mobile);
                printf("check_user(): mobile btaddr: %s, port: %d\n", addr, p);
                printf("check_user(): key length: %d, key: %s\n", key_len, k);

//zapisz btaddr, port i klucz do adresow z argumetu
                strcpy(btaddr,addr);
				strcpy(key,k); //seg fault!
				//key = k; 
                *port = p;
				
                return 0;
            }

            //for (k = 0; ini_getkey(section, k, str, sizearray(str), inifile) > 0; k++) {
            //  printf("\t%s\n", str);
            //} /* for */
        }
      } /* for */
return -1;
}



