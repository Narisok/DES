#include <DES.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum modes
{
    encrypt = 0x1 | 0x1<<1,
    decrypt = 0x1<<1 | 0x1<<2,

    padding_PKCS7 = 0x1<<3 | 0x1<<4,
    padding_ISO10126 = 0x1<<3 | 0x1<<5

    
};

int main(int argc, char **argv)
{
    enum modes mode = encrypt|padding_PKCS7;
    int is_decrypting = 0;
    size_t buff_size = 16/* *256 */;

    for(int i = 1; i < argc; ++i) {
        if(!strcmp( argv[i], "-d")) {
            is_decrypting = 1;
        }
    }

    size_t out_buff_size = mode&decrypt ? DES_calculate_decrypted_size(buff_size) : DES_calculate_encrypted_size(buff_size);
    unsigned char *data_buffer = (unsigned char*)malloc(buff_size);
    unsigned char *out_buffer = (unsigned char*)malloc(out_buff_size);
    if(!data_buffer || !out_buffer) {
        fprintf(stderr, "Bad malloc\n");
        return -1;
    }

    uint64_t key = 0x1234567812345678;
    uint64_t keys[16];
    DES_keys_generate(key, keys);
    if(is_decrypting) {

        unsigned char *first_buffer = data_buffer;
        unsigned char *second_buffer = data_buffer + buff_size/2;
        size_t first_buffer_readed = 0, second_buffer_reader = 0;
        size_t out_size;
        int current_first = 1;

        while(!feof(stdin)) {
            if(current_first) {
                first_buffer_readed = fread(first_buffer, 1 ,buff_size/2, stdin);
            } else {
                second_buffer_reader = fread(second_buffer, 1 ,buff_size/2, stdin);
            }

            if(current_first) {
                if(second_buffer_reader > 0) {
                    out_size = DES_decrypt_ecb(second_buffer, second_buffer_reader, out_buffer, keys, first_buffer_readed?NULL:DES_padding_decrypt_PKCS7);
                    fwrite(out_buffer,1,out_size, stdout);
                } else if(first_buffer_readed != buff_size/2) {
                    out_size = DES_decrypt_ecb(first_buffer, first_buffer_readed, out_buffer, keys, DES_padding_decrypt_PKCS7);
                    fwrite(out_buffer,1,out_size, stdout);
                }
            } else {
                if(first_buffer_readed > 0) {
                    out_size = DES_decrypt_ecb(first_buffer, first_buffer_readed, out_buffer, keys, second_buffer_reader?NULL:DES_padding_decrypt_PKCS7);
                    fwrite(out_buffer,1,out_size, stdout);
                }else if(second_buffer_reader != buff_size/2) {
                    out_size = DES_decrypt_ecb(second_buffer, second_buffer_reader, out_buffer, keys, DES_padding_decrypt_PKCS7);
                    fwrite(out_buffer,1,out_size, stdout);
                }
            }
            
            current_first = !current_first;
        }


    } else {
        while(!feof(stdin)) {
            size_t readed_size = fread(data_buffer, 1 ,buff_size, stdin);
            size_t out_size;

            if(readed_size == buff_size) {
                out_size = DES_encrypt_ecb(data_buffer, readed_size, out_buffer, keys, NULL);
            } else {
                out_size = DES_encrypt_ecb(data_buffer, readed_size, out_buffer, keys, DES_padding_encrypt_PKCS7);
            }
            fwrite(out_buffer,1,out_size, stdout);

        };
}
}