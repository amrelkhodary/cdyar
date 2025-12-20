#ifndef H_CDYAR_ARITHMETIC
#define H_CDYAR_ARITHMETIC
#include "./cdyar_error.h"
#include "./cdyar_structures.h"
#include <limits.h> //for INT_MAX
#include <stdarg.h> //for variadic check_overflow function
#include <stdint.h> //for SIZE_MAX
#include <stdlib.h> //for size_t
void cdyar_check_sizet_overflow(const size_t count, cdyar_returncode *code,
                                ...);
void cdyar_check_uint_overflow(const size_t count, cdyar_returncode *code, ...);
void cdyar_uintpow(const unsigned int base, const unsigned int exponent,
                   unsigned int *outptr, cdyar_returncode *code);
#endif
