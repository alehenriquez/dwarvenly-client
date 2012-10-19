#ifndef HASH_H
#include <ds/hash.h>
#endif

#define get16bits(d) (* ((uint16 *)(d)) )

uint32 map_hash(ptr data, uint32 len) {
    // I've modified this function from the original for cache friendliness and more bit spread.
    // Pray I do not alter it further. - ABC
    if (data == 0 || len == 0) return 0;

    byte *bytes = data;
    uint16 t1;
    uint16 t2;
    uint16 t3;
    uint16 t4;
    uint32 hash = len;
    uint32 rem = len & 7;
    uint64 count = len;
    count >>= 3;

    /* Main loop */
    while (count>0) {
        t1 = get16bits(bytes);
        t2 = get16bits(bytes+2);
        t3 = get16bits(bytes+4);
        t4 = get16bits(bytes+6);
        hash  += t1;
        hash   = (hash << 16) ^ ((t2 << 11) ^ hash);
        hash  += hash >> 11;
        hash  += t3;
        hash   = (hash << 16) ^ ((t4 << 11) ^ hash);
        hash  += hash >> 11;
        bytes  += 8;
        --count;
    }

    /* Handle end cases */
    switch (rem) {
        case 7:
            hash += bytes[6];
            hash ^= hash << 12;
            hash += hash >> 1;
        case 6:
            hash ^= bytes[5] << 7;
            hash ^= hash << 10;
            hash += hash >> 18;
        case 5:
            hash += bytes[4];
            hash ^= hash << 9;
        case 4:
            hash += bytes[3] << 11;
            hash ^= hash << 7;
        case 3:
            hash ^= bytes[2] << 18;
            hash += hash >> 11;
        case 2:
            hash += bytes[1];
            hash ^= hash << 11;
            hash += hash >> 17;
        case 1:
            hash += bytes[0];
            hash ^= hash << 10;
            hash += hash >> 1;
    }

    /* Force "avalanching" of final 127 bits */
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 4;
    hash += hash >> 17;
    hash ^= hash << 25;
    hash += hash >> 6;

    return hash;
}
