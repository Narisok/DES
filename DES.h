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
uint64_t DES_encode_block(uint64_t  block, const uint64_t keys48b[16]);

/**
 * @brief 
 * 
 * @param block 
 * @param keys48b 
 * @return uint64_t 
 */
uint64_t DES_decode_block(uint64_t block, const uint64_t keys48b[16]);


/**
 * @brief 
 * 
 * @param data 
 * @param size_max_7_bytes 
 * @return uint64_t 
 */
uint64_t DES_padding_PKCS7(const uint8_t *data, size_t size_max_7_bytes);

uint64_t DES_padding_ISO10126(uint64_t block);

/**
 * @brief 
 * 
 * @param data 
 * @param bytes_count 
 * @param encoded_data 
 */
size_t DES_encode_ecb(const uint8_t *data, size_t bytes_count, uint8_t *encoded_data, const uint64_t keys48b[16], uint64_t(*padding)(const uint8_t*, uint8_t));

void DES_encode_cbc(const uint8_t *data, size_t bytes_count, uint8_t *encoded_data, uint64_t keys48b[16]);

size_t DES_calculate_encoded_size(size_t bytes_count);//          { return bytes_count - (bytes_count%8) + 8; }
size_t DES_calculate_decoded_size(size_t encoded_bytes_count);//  { return encoded_bytes_count; }

#ifdef __cplusplus
}
#endif

#endif // guard