#ifndef __RCT__DLL_H___74003225423022___
#define __RCT__DLL_H___74003225423022___ 1

#include "shared.h"

typedef struct dll dll;

dll * dll_init(void);
void dll_free(void *list);
int dll_prepend(dll *const list, void *data);
int dll_append(dll *const list, void *data);
int dll_insert(dll *const list, int index, void *data);
void * dll_first(dll *const list);
void * dll_last(dll *const list);
void * dll_get(dll *const list, int index);
int dll_size(dll *const list);
int dll_isempty(dll *const list);
int dll_foreach(dll *const list, gcb callback);

#endif