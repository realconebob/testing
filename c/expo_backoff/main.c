#include <time.h>
#include <sodium.h>
#include <stdio.h>

int nondeterministic_expobackoff(int (*callback)(void*), void *cbargs, int maxtries, int maxslots, const struct timespec default_slot) {
    if(sodium_init() < 0)
        return -1;
    
    int retval = -1;
    struct timespec p = default_slot;

    int sleepmod = 1;
    for(int tries = 0, slots = 1; (retval = callback(cbargs)) != 0 && tries < maxtries; tries++, slots *= 2) {
        if(slots > maxslots)
            slots = maxslots;

        if(slots > 1) {
            sleepmod = randombytes_uniform(slots);
            p.tv_nsec *= sleepmod;
            p.tv_sec *= sleepmod;
        }

        printf("Try: %d, Number of Slots: %d\nNum seconds to wait: %ld, Num nanoseconds to wait: %ld\n", tries + 1, slots - 1, p.tv_sec, p.tv_nsec);

        nanosleep(&p, NULL);
        p = default_slot;
    }

    return retval;

}

int deterministic_expobackoff() {

}


int lmao(void *passed) {
    char *str = (char*)passed;
    return printf("%s\n", str);
}


int main() {
    struct timespec jej = {
        .tv_nsec = 0,
        .tv_sec = 1
    };
    nondeterministic_expobackoff(lmao, (void*)"lol", 25, 10, jej);

    return 0;
}