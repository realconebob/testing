#ifndef ___CONPRO__SHARED_C__160262080730211__
#define ___CONPRO__SHARED_C__160262080730211__

#define DO_PRAGMA(x) _Pragma(#x)
#define CLANG_IGNORE_WARNING(warnoption, ...) \
    DO_PRAGMA(gcc diagnostic push) \
    DO_PRAGMA(gcc diagnostic ignored #warnoption) \
    __VA_ARGS__ \
    DO_PRAGMA(gcc diagnostic pop)

#endif