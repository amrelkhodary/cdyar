#ifndef H_CDYAR_DARRAY
#define H_CDYAR_DARRAY
#include "./cdyar_error.h" //to be able to use cdyar_returncode type + to access error return codes
#include <stdlib.h> //to be able to use size_t
#include "./cdyar_structures.h" //for cdyar_flag

// dynamic array type
typedef struct cdyar_darray {
  void *elements;
  size_t length;
  size_t typesize;
  cdyar_flag flags;
} cdyar_darray;

cdyar_darray *cdyar_narr(const size_t typesize, const size_t length,
                         const cdyar_flag flags, cdyar_returncode *code);

void cdyar_darr(cdyar_darray* arr, cdyar_returncode* code);
#endif
