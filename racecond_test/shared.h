#ifndef __RCT__SHARED_H___76841642717983___
#define __RCT__SHARED_H___76841642717983___ 1

#define ERRRET(eval, rval) do {errno = eval; return rval;} while (0)
typedef int (*gcb)(void*);
typedef void (*fcb)(void*);


typedef struct gdata gdata;

gdata * gd_init(void *data, fcb freer);
void gd_free(void *gd);
void * gd_get(gdata *gd);
int gd_set(gdata *gd, void *data);
fcb gd_getcb(gdata *gd);
int gd_setcb(gdata *gd, fcb newcallback);

#endif