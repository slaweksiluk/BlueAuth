#include <stdio.h>
#include <string.h>
#include "blue_auth.h"
#include "../minini/minIni.h"

#define sizearray(a)  (sizeof(a) / sizeof((a)[0]))



int check_user(char **user, char* btaddr, int *port){
    const char inifile[] = "../users.cfg";
    const char inifile_mobiles[] = "../mobiles.cfg";
    char section[50]; //nazwa sekcji
    char mobile[50]; //nazwa telefonu dla uzytkownika
    char addr[50];  //
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

                printf("user %s enabled with mobile: %s\n", *user, mobile);
                printf("mobile btaddr: %s, port: %d\n", addr, p);
//zapisz btaddr i port do adresow z argumetu
                strcpy(btaddr,addr);
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



