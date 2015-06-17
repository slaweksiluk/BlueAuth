#include <stdio.h>
#include "../pam/blue_auth.h"
#include <stdlib.h>

int main(){
//char* ct = "Ll/rBt684VpYd9ntm1To57AA1w4M7ykjio7mEGY6KLoRpXVoWD2L9pcWnqhG7vKbn7DtcJKHq0hvlvhJQ6yU33ilbornCRgvW5gdbVIjskjh2JhIIV+/J3pwhw+gmaxhzLAwlbqN1nmL3jYAQ9ZhQMvpDQKKof3+mzc6nYotlx8=";
char* ct = "GN5BLPhTkxvZq6AArWlInTGmzD4OfcvgwDl2Nq8b2Lmn+ZA2W39fSgsPlTBI9S1TLey9fmGXkphNrx0vDmP5MC/tTIONS5OYh3Bdyx9LofT5sj+OeMQAGoTtIGlPOr+KSKCjwi3VPh7mb3xCwHmITzY9hz66OeWtb4XjS7Bbm8c=";
    char* priv_key_dir = "../etc/BlueAuth/keys/wave.pem";
char* key_id_hash_dir = "../etc/BlueAuth/hashes/wave";
//char* session_id_b64 = "mEqdOVZNd8Y=";
char* session_id_b64 = "+Z5rzdA2pXA=";
unsigned char* session_id = malloc(SESSION_ID_LEN_BYTES);

// Zdekoduj z B6
size_t decoded_len;
Base64Decode(session_id_b64, &session_id, &decoded_len);
printf("session id decoded len %d\n", (int)decoded_len);


int rc = verify_ciphertext(ct, priv_key_dir, key_id_hash_dir, session_id);

if(rc < 0)
    print_error(rc);
else
    printf("Cipher text test passed!\n");

return 0;
}



