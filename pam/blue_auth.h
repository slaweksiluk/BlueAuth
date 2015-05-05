#ifndef BLUE_AUTH_H_INCLUDED
#define BLUE_AUTH_H_INCLUDED
#include <stdbool.h>
//int check_user(char **user, char* btaddr, int *port);
int check_user(const char **user, char* btaddr, int *port, char *key);
//int rfcomm_client(const char *dest, const int port);
int rfcomm_client(const char *dest, const int port, const int key_len, char *key_rec);
int get_key_len(const char **user);
bool file_exists(const char *f);

// constants
static const char inifile_users[] = "/etc/blueAuth/users.conf";
static const char inifile_mobiles[] = "/etc/blueAuth/mobiles.conf";
#endif
