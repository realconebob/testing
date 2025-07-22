#ifndef __RCT__MONITOR_H___21749706528951___
#define __RCT__MONITOR_H___21749706528951___ 1

// A monitor is just a combination of conditional, mutex, and data. 

#include "shared.h"
#include <threads.h>

// Prefix Nomenclature: 
    // b - blocking; Will wait on mutex availability
    // t - try-block / nonblocking; Exits early if mutex is unavailable
    // u - unsafe; Functions only to run while the mutex is locked, does not lock mutex for user
    // <None> - normal; Functions either unconcerned with blocking (such as init), functions that specify whether to block or not (such as dolocked), or functions obvious in their blocking/non-blocking -ness (such as lock, unlock, wait, etc.)

// If flag is non-zero, it indicates the variable name. Ex, `flag nb` is whether to use non-blocking functions or not

typedef char flag;
typedef struct monitor monitor;
enum M_SIGTYPE {
    M_SIGTYPE_NONE = 1,
    M_SIGTYPE_SIG,
    M_SIGTYPE_BC
};

// Minimal set of functions

monitor * m_init(void *data);

int m_ufinish(monitor *m, flag cancel);
int m_finish(monitor *m, flag cancel, flag nb);

int m_bcancel(monitor *m);
int m_tcancel(monitor *m);
int m_bdone(monitor *m);
int m_tdone(monitor *m);

void m_free(void *m, flag nb);
void m_bfree(void *m);
void m_tfree(void *m);

int m_lock(monitor *m);
int m_unlock(monitor *m);

int m_wait(monitor *m);
int m_signal(monitor *m);
int m_broadcast(monitor *m);

int m_dolocked(monitor *m, flag nb, enum M_SIGTYPE sig, gcb callback);

void * m_uget(monitor *m);
int m_uset(monitor *m, void *data);
int m_uisdone(monitor *m);
int m_uiscanceled(monitor *m);

int m_register(monitor *m, thrd_t * const thread);
int m_cregister(monitor *m, gcb thread_callback);

#endif