#include "DES.h"

#define PERMUTATION(block, new_block, bits_count, table)  {                             \
    new_block = 0x0;                                                                    \
    for(size_t i = 0; i < (bits_count); ++i) {                                          \
        new_block |= (block >> ((bits_count) - table[i]) & 1 )  << ((bits_count)-1-i);  \
    }                                                                                   \
}

#define PERMUTATION_LOOP(block, new_block, table_size, table, bits_in_block)  {            \
    new_block = 0x0;                                                                       \
    for(size_t i = 0; i < (table_size); ++i) {                                             \
        new_block |= (block >> ((bits_in_block) - table[i]) & 1 )  << ((table_size)-1-i);  \
    }                                                                                      \
} 

#define DIVIDE_TWO(block, left, right, size) {   \
    left = 0x0;                                  \
    right = 0x0;                                 \
    left = block >> (size) & ~(~1<<((size)-1));  \
    right = block & ~(~1<<((size)-1));           \
}

#define LEFT_CYCLE_SHIFT(block, block_size, shift) {                                                              \
    block = (( block << (shift) &  ~(~0<<(block_size)) ) | ( block >> (block_size - shift) &  ~(~0<<(shift)) ));  \
}

#define COMBINE_TWO(left, right, block, part_size) {  \
    block = left;                                     \
    block <<= (part_size);                            \
    block |= right;                                   \
}

//=============================> DES tables <========================//

static const uint8_t IP_table[64] = {
    58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9,  1, 59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7 
};

static const uint8_t IP_table_reverse[64] = {
    40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9,  49, 17, 57, 25
};

static const uint8_t SHIFT_table[16] = {
    1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

static const uint8_t PC1_table[56] = {
    57, 49, 41, 33, 25, 17, 9,  1,  58, 50, 42, 34, 26, 18,
    10, 2,  59, 51, 43, 35, 27, 19, 11, 3,  60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15, 7,  62, 54, 46, 38, 30, 22,
    14, 6,  61, 53, 45, 37, 29, 21, 13, 5,  28, 20, 12, 4
};

static const uint8_t PC2_table[48] = {
    14, 17, 11, 24, 1,  5,   3,  28, 15, 6,  21, 10, 23, 19, 12, 4,
    26, 8,  16, 7,  27, 20,  13, 2,  41, 52, 31, 37, 47, 55, 30, 40,
    51, 45, 33, 48, 44, 49,  39, 56, 34, 53, 46, 42, 50, 36, 29, 32
};

//=============================> DES keys generation <========================//

static inline uint64_t PC1(uint64_t key64b);
static inline uint64_t PC2(uint64_t CiDi);

void DES_keys_generate(uint64_t key, uint64_t keys48b[16])
{
    uint64_t key56b = PC1(key);

    uint32_t C,D;
    DIVIDE_TWO(key56b,C,D,28);

    for(size_t i = 0; i < 16; ++i) {
        LEFT_CYCLE_SHIFT(C,28,SHIFT_table[i])
        LEFT_CYCLE_SHIFT(D,28,SHIFT_table[i])

        uint64_t CiDi = 0x0;
        COMBINE_TWO(C,D,CiDi,28)

        keys48b[i] = PC2(CiDi);
    }

}

//=============================> DES one block encode <========================//

static uint64_t InitialPermutation(uint64_t block);
static uint64_t InitialPermutationReverse(uint64_t block);


// static void FeistelRound(uint32_t *L, uint32_t *R, uint64_t key48b);
// static uint32_t F(uint32_t block, uint64_t key48b);

uint64_t DES_encode_block(uint64_t  block, uint64_t keys48b[16])
{
    return 0x0;
}


//=============================> DES definition of functions <========================//

static inline uint64_t PC1(uint64_t key64b)
{
    uint64_t key56b;
    PERMUTATION_LOOP(key64b, key56b, 56, PC1_table, 64)
    return key56b;
}

static inline uint64_t PC2(uint64_t CiDi)
{
    uint64_t key48b;
    PERMUTATION_LOOP(CiDi, key48b, 48, PC2_table, 56)
    return key48b;
}



static uint64_t InitialPermutation(uint64_t block)
{
    uint64_t newBlock;
    PERMUTATION(block, newBlock, 64, IP_table)
    return newBlock;
}

static uint64_t InitialPermutationReverse(uint64_t block)
{
    uint64_t newBlock;
    PERMUTATION(block, newBlock, 64, IP_table_reverse)
    return newBlock;
}