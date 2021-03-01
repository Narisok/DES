#include <stddef.h>
#include <stdint.h>

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
uint64_t DES_encode_block(uint64_t  block, uint64_t keys48b[16]);