#include <stdio.h>
#include "../pam/blue_auth.h"


int main(){
char* ct = "Ll/rBt684VpYd9ntm1To57AA1w4M7ykjio7mEGY6KLoRpXVoWD2L9pcWnqhG7vKbn7DtcJKHq0hvlvhJQ6yU33ilbornCRgvW5gdbVIjskjh2JhIIV+/J3pwhw+gmaxhzLAwlbqN1nmL3jYAQ9ZhQMvpDQKKof3+mzc6nYotlx8=";
char* priv_key_dir = "../etc/BlueAuth/keys/wave.pem";
char* key_id_hash_dir = "../etc/BlueAuth/hashes/wave";
char* session_id_b64 = "mEqdOVZNd8Y=";

int rc = verify_ciphertext(ct, priv_key_dir, key_id_hash_dir, session_id_b64);

if(rc < 0)
    print_error(rc);
else
    printf("Cipher text test passed!\n");

return 0;
}



