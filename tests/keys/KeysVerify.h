#include <DES.h>

inline bool keysVerifiTests()
{
    bool res = true;
    uint64_t key = 0xAABB09182736CCDD;
    uint64_t keys[16], tkeys[16];
    DES_keys_generate(key, keys);
    tkeys[0] = 0x194CD072DE8C;
    tkeys[1] = 0x4568581ABCCE;
    tkeys[2] = 0x06EDA4ACF5B5;
    tkeys[3] = 0xDA2D032B6EE3;
    tkeys[4] = 0x69A629FEC913;
    tkeys[5] = 0xC1948E87475E;
    tkeys[6] = 0x708AD2DDB3C0;
    tkeys[7] = 0x34F822F0C66D;
    tkeys[8] = 0x84BB4473DCCC;
    tkeys[9] = 0x02765708B5BF;
    tkeys[10] = 0x6D5560AF7CA5;
    tkeys[11] = 0xC2C1E96A4BF3;
    tkeys[12] = 0x99C31397C91F;
    tkeys[13] = 0x251B8BC717D0;
    tkeys[14] = 0x3330C5D9A36D;
    tkeys[15] = 0x181C5D75C66D;
    for(int i = 0; i < 16; ++i) {
        if(keys[i] != tkeys[i]) {
            res = false;
        }
    }
    return res;
}