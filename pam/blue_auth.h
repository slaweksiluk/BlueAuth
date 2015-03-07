#ifndef BLUE_AUTH_H_INCLUDED
#define BLUE_AUTH_H_INCLUDED
//int check_user(char **user, char* btaddr, int *port);
int check_user(const char **user, char* btaddr, int *port, char *key);
//int rfcomm_client(const char *dest, const int port);
int rfcomm_client(const char *dest, const int port, const int key_len, char *key_rec);
int get_key_len(const char **user);

// constants
static const char inifile[] = "../users.cfg";
static const char inifile_mobiles[] = "../mobiles.cfg";
#endif
