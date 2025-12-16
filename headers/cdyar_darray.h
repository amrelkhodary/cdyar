#ifndef H_CDYAR_DARRAY
#define H_CDYAR_DARRAY
#define CDYAR_DEFAULT_RESIZE_POLICY NULL

#include "./cdyar_error.h" //to be able to use cdyar_returncode type + to access error return codes
#include "./cdyar_structures.h" //for cdyar_flag
#include <stdlib.h>             //to be able to use size_t

// dynamic array type
typedef struct cdyar_darray {
  void *elements;
  size_t length;
  size_t typesize;
  cdyar_flag flags;
  cdyar_resizepolicy policy;
  cdyar_typehandler handler;
} cdyar_darray;

// functions for memory allocation and deallocation
cdyar_darray *cdyar_narr(const size_t typesize, const size_t length,
                         const cdyar_resizepolicy policy,
                         const cdyar_typehandler handler,
                         const cdyar_flag flags, cdyar_returncode *code);
void cdyar_darr(cdyar_darray *arr, cdyar_returncode *code);

// setter and getter functions for safe array access and automatic bounds
// checking
void cdyar_set(cdyar_darray *arr, const size_t index, void *valueptr,
               cdyar_returncode
                   *code); // used void* for value because the type is not known
void cdyar_get(cdyar_darray *arr, const size_t index, void *outptr,
               cdyar_returncode *code);
#endif
