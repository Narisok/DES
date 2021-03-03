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

#define DIVIDE_TWO(block, left, right, size) {      \
    left = 0x0;                                     \
    right = 0x0;                                    \
    left = block >> (size);                         \
    right = block;                                  \
}

#define DIVIDE_TWO_CLEAR(block, left, right, size) {  \
    left = 0x0;                                       \
    right = 0x0;                                      \
    left = block >> (size);                           \
    left = ~(~(left&0)<<(size)) & left;               \
    right = block;                                    \
    right = ~(~(right&0)<<(size)) & right;            \
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

static const uint8_t E_table[48] = {
    32,     1,  2,  3,  4,      5,
    4,      5,  6,  7,  8,      9,
    8,      9,  10, 11, 12,     13,
    12,     13, 14, 15, 16,     17,
    16,     17, 18, 19, 20,     21,
    20,     21, 22, 23, 24,     25,
    24,     25, 26, 27, 28,     29,
    28,     29, 30, 31, 32,     1
};


static const uint8_t S_table[8][4][16] = {
    {
        {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
        {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
        {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
        {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
    },
    {
        {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
        {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
        {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
        {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
    },
    {
        {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
        {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
        {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
        {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
    },
   { 
        {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15 },
        {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9 },
        {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4 },
        {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14 }
    },
    {
        {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9 },
        {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6 },
        {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14 },
        {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3 }
    },
    {
        {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11 },
        {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8 },
        {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6 },
        {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13 }
    },
    {
        {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1 },
        {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6 },
        {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2 },
        {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12 }
    },
    {
        {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7 },
        {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2 },
        {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8 },
        {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
    }
};

static const uint32_t P_table[32] = {
    16, 7, 20, 21, 
    29, 12, 28, 17,
    1, 15, 23, 26, 5, 18, 31, 10,
    2, 8, 24, 14, 32, 27, 3, 9,
    19, 13, 30, 6, 22, 11, 4, 25
};


//=============================> DES keys generation <========================//

static inline uint64_t PC1(uint64_t key64b);
static inline uint64_t PC2(uint64_t CiDi);

#ifdef __cplusplus
extern "C"
#endif
void DES_keys_generate(uint64_t key, uint64_t keys48b[16])
{
    uint64_t key56b = PC1(key);

    uint32_t C,D;
    DIVIDE_TWO_CLEAR(key56b,C,D,28);

    for(size_t i = 0; i < 16; ++i) {
        LEFT_CYCLE_SHIFT(C,28,SHIFT_table[i])
        LEFT_CYCLE_SHIFT(D,28,SHIFT_table[i])

        uint64_t CiDi = 0x0;
        COMBINE_TWO(C,D,CiDi,28)

        keys48b[i] = PC2(CiDi);
    }

}

//=============================> DES one block encode/decode <========================//

static inline uint64_t IP(uint64_t block);
static inline uint64_t IP_reverse(uint64_t block);
static inline uint32_t F(uint32_t Ri, uint64_t key48b);

#ifdef __cplusplus
extern "C"
#endif
uint64_t DES_encode_block(uint64_t  block, uint64_t keys48b[16])
{
    block = IP(block);
    uint32_t Li, Ri;
    DIVIDE_TWO(block, Li, Ri, 32)

    for(unsigned i = 0; i < 16; ++i) {
        uint32_t Li_plus = Ri;
        uint32_t Ri_plus = Li ^ F(Ri, keys48b[i]);
        Li = Li_plus;
        Ri = Ri_plus;
    }

    COMBINE_TWO(Ri, Li, block, 32)
    block = IP_reverse(block);

    return block;
}

#ifdef __cplusplus
extern "C"
#endif
uint64_t DES_decode_block(uint64_t block, uint64_t keys48b[16])
{
    block = IP(block);

    uint32_t Li, Ri;
    DIVIDE_TWO(block, Li, Ri, 32)

    for(unsigned i = 0; i < 16; ++i) {
        uint32_t Li_minus = Ri;
        uint32_t Ri_minus = Li ^ F(Ri, keys48b[15-i]);
        Li = Li_minus;
        Ri = Ri_minus;
    }

    COMBINE_TWO(Ri, Li, block, 32)
    block = IP_reverse(block);
    return block;
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



static inline uint64_t IP(uint64_t block)
{
    uint64_t newBlock;
    PERMUTATION(block, newBlock, 64, IP_table)
    return newBlock;
}

static inline uint64_t IP_reverse(uint64_t block)
{
    uint64_t newBlock;
    PERMUTATION(block, newBlock, 64, IP_table_reverse)
    return newBlock;
}


static inline uint64_t E(uint32_t Ri);
static inline uint32_t S(uint64_t B8);
static inline uint32_t P(uint32_t raw);

static inline uint32_t F(uint32_t Ri, uint64_t key48b)
{
    uint64_t exp48b = E(Ri);
    uint64_t B8     = exp48b ^ key48b;
    uint32_t result = S(B8);
    return            P(result);
}

static inline uint64_t E(uint32_t Ri)
{
    uint64_t exp48b = 0x0;
    for(size_t i = 0; i < 48; ++i) {            
        exp48b |= (uint64_t)(Ri >> (32-E_table[i]) & 1 )  << (47-i);         
    }
    return exp48b;
}

static inline uint32_t S(uint64_t B8)
{
    uint32_t res = 0x0;
    for(int i = 0; i < 8; ++i) {
        uint8_t Bi = B8 >> (42 - i*6) & 0b00111111;

        uint8_t row = (Bi>>4&2) | (Bi&1) ;
        uint8_t col = (Bi>>1)&0xf;
        uint32_t S = S_table[i][row][col];
       

        res |= S << (28 - i *4);
    }
    return res;
}

static inline uint32_t P(uint32_t raw)
{
    uint32_t res;
    PERMUTATION(raw, res, 32, P_table)
    return res;
}