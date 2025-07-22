#ifndef ___SIGNATURE_FORCING__MAIN_H___3630100604876___
#define ___SIGNATURE_FORCING__MAIN_H___3630100604876___ 1

// int global = 0;
// do {global++;} while (0);
	// As expected, I can't just throw out code outside of scope and expect it to work

#include <stdlib.h>
#define FORCETYPE(check, type) _Generic((check), type: 1, default: abort())

#define ___defsig(func, sig) _Static_assert(_Generic((func), (sig): 1, default: 0), "invalid function signature")

#endif
