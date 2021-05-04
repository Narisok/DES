#ifndef DES_NARISOK_GUARD
#define DES_NARISOK_GUARD

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 
 * 
 * @param key 
 * @param keys48b 
 */
void DES_keys_generate(uint64_t key, uint64_t keys48b[16]);

/**
 * @brief 
 * 
 * @param block 
 * @param key 
 * @return uint64_t 
 */
uint64_t DES_encrypt_block(uint64_t  block, const uint64_t keys48b[16]);

//TODO aliases for encode -> encrypt

/**
 * @brief 
 * 
 * @param block 
 * @param keys48b 
 * @return uint64_t 
 */
uint64_t DES_decrypt_block(uint64_t block, const uint64_t keys48b[16]);


/**
 * @brief 
 * 
 * @param data 
 * @param size_max_7_bytes 
 * @return uint64_t 
 */
uint64_t DES_padding_encrypt_PKCS7(const uint8_t *data, size_t size_max_7_bytes);

/**
 * @brief 
 * 
 * @param last_block 
 * @param out_buffer_max_8_bytes 
 * @return size_t bytes count that were written to the buffer 
 */
int DES_padding_decrypt_PKCS7(const uint64_t last_block, uint8_t *out_buffer_min_7_bytes);


uint64_t DES_padding_ISO10126(uint64_t block); //TODO to do

/**
 * @brief 
 * 
 * @param input_data 
 * @param bytes_count 
 * @param out_buffer 
 * @param keys48b 
 * @param padding 
 * @return size_t 
 */
size_t DES_encrypt_ecb(const uint8_t *input_data, size_t bytes_count, uint8_t *out_buffer, const uint64_t keys48b[16], uint64_t(*padding)(const uint8_t*, size_t));

/**
 * @brief 
 * 
 * @param input_data 
 * @param bytes_count 
 * @param out_buffer 
 * @param keys48b 
 * @param padding 
 * @return size_t 
 */
size_t DES_decrypt_ecb(const uint8_t *input_data, size_t bytes_count, uint8_t *out_buffer, const uint64_t keys48b[16], int(*padding)(const uint64_t, uint8_t*));

void DES_encode_cbc(const uint8_t *data, size_t bytes_count, uint8_t *encoded_data, uint64_t keys48b[16]);

size_t DES_calculate_encrypted_size(size_t decrypted_bytes_count);//          { return bytes_count - (bytes_count%8) + 8; }
size_t DES_calculate_decrypted_size(size_t encrypted_bytes_count);//  { return encoded_bytes_count; }

#ifdef __cplusplus
}
#endif

#endif // guard