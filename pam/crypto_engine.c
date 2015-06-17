#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/buffer.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include "blue_auth.h"

void printLastError(char *msg)
{
    char * err = malloc(130);
    ERR_load_crypto_strings();
    ERR_error_string(ERR_get_error(), err);
    printf("%s ERROR: %s\n",msg, err);
    free(err);
}

 
//  Na wejscie otrzymuje szyfrogram B64 i sciezke do klucza prywatnego
//   naa tej podstawie generuje odszyfrowana wiadomosc i zapsisuje ja 
//   pod wskaznikiem plain_text
unsigned char* decrypt(const char *ct, const char* priv_key_dir){
    printf("   decrypt() fun started...\n");
    const static int padding = RSA_PKCS1_OAEP_PADDING;
    unsigned char* decrypted = malloc(sizeof(unsigned char)*(KEY_ID_LEN_BYTES + 
        SESSION_ID_LEN_BYTES));    
    unsigned char* bin_to_dec;
    FILE *fp;
    EVP_PKEY *pkey = NULL;
    RSA* rsa = NULL;
    size_t test;
    

    //int n = sizeof(ct) / sizeof(unsigned char);
    //printf("Ciphertext size: %d\n", n);
    printf("Ciphertext to decrypt: %.10s...\n", ct);
    

    //  Zdekoduj szyfroggram do postaci binarnej
    Base64Decode(ct, &bin_to_dec, &test);
    
    //  Odczytaj plik i pobierz z niego klucz prywatny
    if (!(fp = fopen(priv_key_dir, "r"))){
            printf("fopen err\n");
            return NULL;
    }

    if (!PEM_read_PrivateKey(fp, &pkey, NULL, NULL)){
            printf("PEM read err\n");
            return NULL;
    }
    fclose(fp);

    //  Utworz obiekt RSA na podstawie klucza
    rsa = EVP_PKEY_get1_RSA(pkey);
    
    int decrypted_length = RSA_private_decrypt(128, bin_to_dec, decrypted, rsa, padding);
    if(decrypted_length == -1){
        printLastError("Private Decrypt failed ");
        return NULL;
    }
    //printf("Decrypted Text =%s\n", decrypted);
    printf("Decrypted data length: %d\n",decrypted_length);    
    
    printf("   decrypt() fun finished...\n");
    return decrypted;
}

unsigned char* calc_hash(unsigned char* mess1,int mess1_len){
printf("   calc_hash() fun started...\n");
EVP_MD_CTX *mdctx;
const EVP_MD *md;
//unsigned char md_value[EVP_MAX_MD_SIZE];
unsigned char* md_value = malloc(sizeof(unsigned char) * EVP_MAX_MD_SIZE);
unsigned int md_len;
int i;

OpenSSL_add_all_digests();


md = EVP_get_digestbyname("sha512");


mdctx = EVP_MD_CTX_create();
EVP_DigestInit_ex(mdctx, md, NULL);
EVP_DigestUpdate(mdctx, mess1, mess1_len);
EVP_DigestFinal_ex(mdctx, md_value, &md_len);
EVP_MD_CTX_destroy(mdctx);

printf("Digest is: ");
for(i = 0; i < 10; i++)
    printf("%02x", md_value[i]);
printf("...\n");

/* Call this once before exit. */
EVP_cleanup();
printf("   calc_hash() fun finished...\n");
return md_value;
}





int verify_ciphertext(const char* ct, const char* priv_key_dir, const char* key_id_hash_dir, 
        unsigned char* session_id){
    
    //unsigned char *session_id = malloc(SESSION_ID_LEN_BYTES);
    unsigned char *rec_key_id = malloc(KEY_ID_LEN_BYTES);
    unsigned char *rec_hash = malloc(HASH_LEN_BYTES);
    
    char* hash_b64_buf = malloc(HASH_LEN_B64);
    unsigned char* hash = malloc(HASH_LEN_BYTES);
    
    unsigned char* decrypted = malloc(SESSION_ID_LEN_BYTES + KEY_ID_LEN_BYTES);
    int fp;
    
    size_t hash_decoded_len;
    //size_t session_id_decoded_len;

    //  Zdekoduj id sesji z B64
    // printf("Decode session ID\n");
    //printf("verifyciphertext():  session id base 64 %s\n", session_id_b64);
    //Base64Decode(session_id_b64, &session_id, &session_id_decoded_len);
    //printf("Session id decoded length: %d\n", (int)session_id_decoded_len);
  
    //  Odczytaj plik i pobierz z niego skrót key ID
    fp = open(key_id_hash_dir, O_RDONLY);
    if(fp < 0){
        return E_HASH_FILE;
    }
    read(fp, hash_b64_buf, (size_t) HASH_LEN_B64);
    
    printf("Hahs B64 : %s\n", hash_b64_buf);
    printf("Hahs B64 len: %d\n", (int) strlen(hash_b64_buf));
    
    printf("Remove last char...\n");
    hash_b64_buf[strlen(hash_b64_buf)-1] = 0;
    printf("Hahs B64 : %s\n", hash_b64_buf);
    printf("Hahs B64 len: %d\n", (int) strlen(hash_b64_buf));
    
    
    //  Zdekoduj skrót z b64
    //printf("Hash: %s\n", hash_b64_buf);
    Base64Decode(hash_b64_buf, &hash, &hash_decoded_len);
    printf("Hash decoded length: %d\n", (int)hash_decoded_len);

    //decrypted = malloc(8*40);
    //printf("Decrypt cipher text\n");
    decrypted = decrypt(ct, priv_key_dir);
    if(decrypted == NULL)
        print_error(E_DECRYPT_ERR);
  
    //printf("Compare session ID\n");
    int i;
    for(i=0; i<8; i++){
        //  bajty session id
        if(decrypted[i] != session_id[i]){
            printf("Session id not equals!\n");
            return E_SESSION_ID_DIFF;
        } 
    }
    printf("Comparing session IDs =>   ***OK***\n");

    //printf("Extract key ID from ct\n");
    rec_key_id = malloc(8*32);
    for(i=0; i<32; i++){
        *(rec_key_id+i) = *(decrypted+i+8);
        //printf("i = %d => %02x\n",i, rec_key_id[i]);
    }

    //  Calculate idkey hash
    rec_hash = calc_hash(rec_key_id, 32);

    //  Porownaj skroty
    for(i=0; i<64; i++){
        //  bajty session id
        //printf("i = %d => %02x +++ %02x\n",i, hash[i], rec_hash[i]);
        if(rec_hash[i] != hash[i]){
            printf("Key IDs hashes not equal!\n");
            return E_HASH_DIFF;
        } 
    }
    printf("Comparing hashes =>   ***OK***\n");
    printf("Success, return 0\n");
    return 0;
}



