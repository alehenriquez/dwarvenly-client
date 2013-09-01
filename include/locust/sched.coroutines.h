#define COROUTINES_H

#ifndef TYPES_H
#include <locust/type.types.h>
#endif

#include <Block.h>

// Macros for raw setup of processes. Inspired by:
// http://www.chiark.greenend.org.uk/~sgtatham/coroutines.html
#define CR_INIT       __block uint32 __cr_state = 1;
#define CR_START      switch(__cr_state) { case 1:
#define CR_YIELD(ret) do { __cr_state=__LINE__; return ret; case __LINE__:; } while (0)
#define CR_STOP(ret)  do { __cr_state=0; return ret; } while(0)
#define CR_FREE(name) Block_release(name)

/* // Just in case I need them...
#define goto do { your statements here; } while(0); goto \
#define return do { your statements here; } while(0); return \
#define continue do { your statements here; continue; } while (0)
#define break do { your statements here; break; } while (0)
*/

/*
// Example usage of coroutine helpers. Must use blocks to encapsulate state. (or some other hack, but ffs...)
typedef uint8(^CountBlock)(void);

CountBlock makeCounter(uint8 initial) {
    __block uint8 counter = initial;
    CR_INIT;

    CountBlock block = ^ {
        CR_START;
        do {
            CR_YIELD(counter++);
        } while(counter != 0);
        CR_STOP(0);
    };

    return Block_copy(block);
}

#define TOTAL 60000000
// 10300
// 3281
int main() {
    CountBlock counters[TOTAL];
    int i;
    for (i=0; i < TOTAL; ++i) {
        counters[i] = makeCounter(0);
    }

    uint8 res;
    for (i=0; i < TOTAL; ++i) {
        res = counters[i]();
    }

    do {
        printf("Counters: %u\n", res);
        for (i=0; i < TOTAL; ++i) {
            res = counters[i]();
        }
    } while(res != 0);

    for (i=0; i<TOTAL; ++i) {
        CR_FREE(counters[i]);
    }
    return 0;
}

*/
