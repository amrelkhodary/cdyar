#include "../headers/cdyar_darray.h"
#include "../headers/cdyar_error.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
    internal function
    check if the flag passed to cdyar_narr is valid combination of binary flags
    args: 1) const cdyar_flags flags: the flag
    returns: (type: cdyar_bool) a boolean indicating whether the flag is valid
   or not
*/
static cdyar_bool areFlagsValid(const cdyar_flag flags) {
  // TODO: implement function

  return cdyar_true;
}

/*
    internal function (type: resizepolicy)
    default resize policy provided by cdyar for the dynamic array data type. It
   performs a 2x resize if the index out of bounds is within 2x of the current
   range.

    argss: 1) cdyar_darray* arr     : a pointer to the dynamic array
           2) const size_t index    : the index the user tried to insert in
           3) cdyar_returncode* code: a pointer to a returncode variable to
   store status returns: void
*/
static void cdyar_default_resize_policy(cdyar_darray *arr, const size_t index,
                                        cdyar_returncode *code) {
  // don't forget to update arr->length after resize!
  // dont't forget to update code!
}

/*
    allocate memory for a new dynamic array
    args: 1) const size_t typesize  : the size of the type to be stored in the
   array 2) const size_t length    : how many elements in the array 3) const
   cdyar_flag flags : binary flags passed 4) cdyar_returncode *code : pointer to
   a returncode variable to report an error (if any)

    returns: (type: cdyar_darray*) a pointer to the newly created dynamic array
   structure
*/
cdyar_darray *cdyar_narr(const size_t typesize, const size_t length,
                         const cdyar_resizepolicy policy,
                         const cdyar_typehandler handler,
                         const cdyar_flag flags, cdyar_returncode *code) {

  // make sure code is not null
  CDYAR_CHECK_CODE(code);

  // make sure length passed is positive
  if (length == 0) {
    *code = CDYAR_INVALID_INPUT;
    return NULL;
  }

  // make sure typesize if a valid size
  if (typesize == 0) {
    *code = CDYAR_INVALID_INPUT;
    return NULL;
  }

  // make sure there is no overflow
  if (length > SIZE_MAX / typesize) {
    *code = CDYAR_INVALID_INPUT;
    return NULL;
  }

  // make sure the flags are valid
  if (!areFlagsValid(flags)) {
    *code = CDYAR_INVALID_INPUT;
    return NULL;
  }

  // make sure handler is not null
  if (!handler) {
    *code = CDYAR_INVALID_INPUT;
    return NULL;
  }

  // allocate memory for the new dynamic array structure
  cdyar_darray *narr = (cdyar_darray *)malloc(sizeof(cdyar_darray));
  if (!narr) {
    *code = CDYAR_MEMORY_ERROR;
    return NULL;
  }

  // allocate memory for the new elements array inside the dynamic array
  // structure
  narr->elements = malloc(length * typesize);
  if (!narr->elements) {
    *code = CDYAR_MEMORY_ERROR;
    free(narr);
    return NULL;
  }

  // zero out all the elements inside the inner array
  memset(narr->elements, 0, length * typesize);

  // set properties
  narr->length = length;
  narr->typesize = typesize;
  narr->flags = flags;

  // assign resize policy
  if (policy == CDYAR_DEFAULT_RESIZE_POLICY) {
    narr->policy = cdyar_default_resize_policy;
  } else {
    narr->policy = policy;
  }

  // assign type handler
  narr->handler = handler;

  *code = CDYAR_SUCCESSFUL;
  return narr;
}

/*
    free allocated memory for a dynamic array
    args: 1) cdyar_darray* arr     : the dynamic array in question
          2) cdyar_returncode* code: a pointer to a returncode variable to
   report any error (if any) returns: void
*/
void cdyar_darr(cdyar_darray *arr, cdyar_returncode *code) {
  // make sure code is not null
  CDYAR_CHECK_CODE(code);

  // make sure arr is not null
  if (!arr) {
    *code = CDYAR_INVALID_INPUT;
    return;
  }

  // if the inner array exists, free it
  if (arr->elements) {
    free(arr->elements);
  }

  // free the dynamic array
  free(arr);
  *code = CDYAR_SUCCESSFUL;
}

/*
    safely set an element at a particular index in a dynamic array to a value
    args: 1) cdyar_darray* arr     : a pointer to the dynamic array
          2) const size_t index    : the index of the element
          3) void *valueptr        : a pointer to a variable that holds the new
   value to be stored in the element 4) cdyar_returncode* code: a pointer to a
   cdyar_returncode variable to store whether the function was successful or not
    returns: void
*/
void cdyar_set(cdyar_darray *arr, const size_t index, void *valueptr,
               cdyar_returncode *code) {
  // check that code is not null
  CDYAR_CHECK_CODE(code);

  // check that arr is not null
  if (!arr) {
    *code = CDYAR_INVALID_INPUT;
    return;
  }

  // check that valueptr is not null
  if (!valueptr) {
    *code = CDYAR_INVALID_INPUT;
    return;
  }

  // check that typesize is not zero
  if (arr->typesize == 0) {
    *code = CDYAR_INVALID_INPUT;
    return;
  }

  // check for typesize overflow
  if (index > SIZE_MAX / arr->typesize) {
    *code = CDYAR_INVALID_INPUT;
    return;
  }

  // check that an elements array actually exists within the dynamic array
  if (!arr->elements) {
    *code = CDYAR_INVALID_INPUT;
    return;
  }

  // make sure a resize policy for the dynamic array exists
  if (!arr->policy) {
    *code = CDYAR_INVALID_INPUT;
    return;
  }

  // make sure a type handler for the dynamic array exists
  if (!arr->handler) {
    *code = CDYAR_INVALID_INPUT;
    return;
  }

  // bounds checking
  if (index >= arr->length) {
    *code = CDYAR_ARR_OUT_OF_BOUNDS;

    // invoke the dynamic array's resize policy
    arr->policy(arr, index, code);
  }

  // perform set operation if index is within bounds or a if the array was
  // successfully resized
  if (*code == CDYAR_SUCCESSFUL) {
    // use a typehandler to perform this type of operation operation
    //((CUSTOM_TYPE*)arr->elements)[index] = *((CUSTOM_TYPE*)valueptr);
    // void ptr casted to char* to suppress compiler warnings
    arr->handler(((char*)(arr->elements)) + (arr->typesize * index), valueptr,
                 CDYAR_DIRECTION_ASSIGN_RIGHT_TO_LEFT, code);
  }
}

void cdyar_get(const cdyar_darray *arr, const size_t index, void *outptr,
               cdyar_returncode *code) {
  // check code is not null
  CDYAR_CHECK_CODE(code);

  // check arr is not null
  if (!arr) {
    *code = CDYAR_INVALID_INPUT;
    return;
  }

  // check *outptr is not null
  if (!outptr) {
    *code = CDYAR_INVALID_INPUT;
    return;
  }

  // check that the typesize is not zero
  if (arr->typesize == 0) {
    *code = CDYAR_INVALID_INPUT;
    return;
  }

  // chcek for typesize overflow
  if (index > SIZE_MAX / arr->typesize) {
    *code = CDYAR_INVALID_INPUT;
    return;
  }

  // check that an elements array actually exists within the static array
  if (!arr->elements) {
    *code = CDYAR_INVALID_INPUT;
    return;
  }

  // check that there is a handler function in the dynamic array
  if (!arr->handler) {
    *code = CDYAR_INVALID_INPUT;
    return;
  }

  // bounds checking
  if (index >= arr->length) {
    *code = CDYAR_ARR_OUT_OF_BOUNDS;
    return;
  }

  // assign outptr to a pointer to the element in question in the array
  // void ptr converted to char* to suppress compiler warnings
  arr->handler(((char*)(arr->elements)) + (arr->typesize * index), outptr,
               CDYAR_DIRECTION_ASSIGN_LEFT_TO_RIGHT, code);

  /**code=CDYAR_SUCCESSFUL*/ // uneccessary since handler will determine code
                             // anyways + it could hide handler failure`
}
