#include <stdio.h>
#include <DES.h>
#include <string.h>

int main()
{
    uint64_t key = 0x1234567812345678;
    uint64_t keys[16];
    DES_keys_generate(key, keys);

    uint8_t out_buffer[100];
    for(int i = 0; i < 100; i++) out_buffer[i] = 'H';
    uint8_t encoded_buffer[100];

    uint8_t data_buffer[] = "12345678aoeusnth";
    printf("Input data: %s\n", data_buffer);

    size_t res = DES_encrypt_ecb(data_buffer, strlen(data_buffer), encoded_buffer, keys, NULL);
    printf("DES encrypt: res<%lu>\n", res);
    res = DES_decrypt_ecb(encoded_buffer, res, out_buffer, keys, NULL);

    out_buffer[res] = '\0';
    printf("DES decrypt: res<%lu> out<%s>\n", res, out_buffer);
    return 0;
}