#include "../headers/cdyar_darray.h"
#include "../headers/cdyar_error.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cdyar_bool areFlagsValid(const cdyar_flag flags) {
    //TODO: implement function
    return cdyar_true;
}

/*
    allocate memory for a new dynamic array
    args: 1) const size_t typesize  : the size of the type to be stored in the array
          2) const size_t length    : how many elements in the array
          3) const cdyar_flag flags : binary flags passed
          4) cdyar_returncode *code : pointer to a returncode variable to report an error (if any)

    returns: (type: cdyar_darray*) a pointer to the newly created dynamic array structure
*/
cdyar_darray *cdyar_narr(const size_t typesize, const size_t length,
                         const cdyar_flag flags, cdyar_returncode *code) {

  //make sure code is not null
  CDYAR_CHECK_CODE(code);

  //make sure length passed is positive
  if(length == 0) {
      *code=CDYAR_INVALID_INPUT;
      return NULL;
  }

  //make sure typesize if a valid size
  if(typesize == 0) {
     *code=CDYAR_INVALID_INPUT;
     return NULL;
  }

  //make sure there is no overflow
  if(length > SIZE_MAX / typesize) {
      *code=CDYAR_INVALID_INPUT;
      return NULL;
  }

  //make sure the flags are valid
  if(!areFlagsValid(flags)) {
     *code=CDYAR_INVALID_INPUT;
     return NULL;
  }

  //allocate memory for the new dynamic array structure
  cdyar_darray *narr = (cdyar_darray *)malloc(sizeof(cdyar_darray));
  if (!narr) {
    *code = CDYAR_MEMORY_ERROR;
    return NULL;
  }

  //allocate memory for the new elements array inside the dyanmic array structure
  narr->elements = malloc(length * typesize);
  if (!narr->elements) {
    *code = CDYAR_MEMORY_ERROR;
    free(narr);
    return NULL;
  }

  //zero out all the elements inside the inner array
  memset(narr->elements, 0, length*typesize);

  //set properties
  narr->length = length;
  narr->typesize = typesize;
  narr->flags = flags;
  *code = CDYAR_SUCCESSFUL;

  return narr;
}

/*
    free allocated memory for a dynamic array
    args: 1) cdyar_darray* arr     : the dyanmic array in question
          2) cdyar_returncode* code: a pointer to a returncode variable to report any error (if any)
    returns: void
*/
void cdyar_darr(cdyar_darray *arr, cdyar_returncode* code) {
    //make sure code is not null
    CDYAR_CHECK_CODE(code);

    //make sure arr is not null
    if(!arr) {
       *code=CDYAR_INVALID_INPUT;
       return;
    }

    //if the inner array exists, free it
    if(arr->elements) {
        free(arr->elements);
    }

    //free the dynamic array
    free(arr);
    *code=CDYAR_SUCCESSFUL;
}
