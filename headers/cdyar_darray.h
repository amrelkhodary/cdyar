#ifndef H_CDYAR_DARRAY
#define H_CDYAR_DARRAY
#define CDYAR_DEFAULT_RESIZE_POLICY NULL
#define CDYAR_DARRAY_FLAG_COUNT 2

#include "./cdyar_arithmetic.h" //for check_sizet_overflow called in cdyar_default_resize_policy
#include "./cdyar_error.h" //to be able to use cdyar_returncode type + to access error return codes
#include "./cdyar_structures.h" //for cdyar_flag
#include "./cdyar_types.h"
#include <math.h>   //for internal function
#include <stdlib.h> //to be able to use size_t

// enumeration of different flags that could be passed to the dynamic array
enum cdyar_darray_binflags {
  CDYAR_ARR_AUTO_RESIZE = 0b1, // automatically resizes the if a set operation
                               // happened outside of bounds

};

// resize policy function type
typedef void (*cdyar_resizepolicy)(struct cdyar_darray *arr, const size_t index,
                                   cdyar_returncode *code);

// dynamic array type
typedef struct cdyar_darray {
  void *elements;
  size_t length;   // the number of elements in the arr
  size_t capacity; // how many elements can the array currently hold
  size_t typesize;
  cdyar_flag flags;
  cdyar_resizepolicy policy;
  cdyar_typehandler handler;
  cdyar_returncode *code;
} cdyar_darray;

// functions for memory allocation and deallocation
cdyar_returncode cdyar_narr(const size_t typesize, const size_t capacity,
                            const cdyar_resizepolicy policy,
                            const cdyar_typehandler handler,
                            const cdyar_flag flags, cdyar_darray *outptr);
cdyar_returncode cdyar_darr(cdyar_darray *arr);



// setter and getter functions for safe array access and automatic bounds
// checking
cdyar_returncode
cdyar_set(cdyar_darray *arr, const size_t index,
          void *valueptr); // used void* for value because the type is not known
cdyar_returncode cdyar_get(const cdyar_darray *arr, const size_t index,
                           void *outptr);

// property setting functions
cdyar_returncode cdyar_setflags(cdyar_darray *arr, const cdyar_flag flags);
cdyar_returncode cdyar_setpolicy(cdyar_darray *arr,
                                 const cdyar_resizepolicy policy);
#endif
