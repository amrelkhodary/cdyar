#include "../headers/cdyar_darray.h"
#include "../headers/cdyar_error.h"
#include <stdarg.h>
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
static void areFlagsValid(const cdyar_flag flags, cdyar_bool *outptr,
                          cdyar_returncode *code) {
  cdyar_flag MAX = 0;
  cdyar_uintpow(2, CDYAR_DARRAY_FLAG_COUNT, &MAX, code);
  MAX -= 1; // setting MAX to 2^(flag_count) - 1

  if (*code != CDYAR_SUCCESSFUL) {
    // an issue occured with cdyar_intpow, propagate the error upwards
    return;
  }

  if (flags <= MAX) {
    *outptr = cdyar_true;
  } else {
    *outptr = cdyar_false;
  }

  *code = CDYAR_SUCCESSFUL;
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

  // check code is not null
  CDYAR_CHECK_CODE(code);

  // check arr is not null
  if (!arr) {
    *code = CDYAR_DYNAMIC_ARR_DOES_NOT_EXIST;
    return;
  }

  // check that there exists a static elements array inside the dynamic array
  // structure
  if (!arr->elements) {
    *code = CDYAR_CORRUPTED_DYNAMIC_ARR;
    return;
  }

  // TODO: Check overflow
  cdyar_check_sizet_overflow(3, code, arr->capacity, 2, arr->typesize);
  if (*code != CDYAR_SUCCESSFUL) {
    return;
  }

  // bounds checking
  // check that index is within the range the warrants a resize as specified by
  // this policy i.e. check that length <= index <= length * 2
  if (!(index >= arr->capacity && index <= arr->capacity * 2)) {
    *code = CDYAR_RESIZE_POLICY_INVALID_RANGE;
    return;
  }

  // resize the array
  void *elements_temp =
      realloc(arr->elements, arr->capacity * arr->typesize * 2);
  if (!elements_temp) {
    *code = CDYAR_MEMORY_ERROR;
    return;
  }

  // zero out the new portion of the array
  arr->elements = elements_temp;
  memset(((char *)arr->elements) + (arr->typesize * arr->capacity), 0,
         (arr->typesize * arr->capacity));

  // make sure to double the capacity
  arr->capacity *= 2;
  *code = CDYAR_SUCCESSFUL;
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
cdyar_returncode cdyar_narr(const size_t typesize, const size_t capacity,
                            const cdyar_resizepolicy policy,
                            const cdyar_typehandler handler,
                            const cdyar_flag flags, cdyar_darray *outptr) {

  // create a cdyar_returncode for the dynamic array so that
  cdyar_returncode *code = malloc(sizeof(cdyar_returncode));
  if (!code) {
    return CDYAR_MEMORY_ERROR;
  }
  *code = CDYAR_SUCCESSFUL; // default value of code

  // make sure capacity passed is positive
  if (capacity == 0) {
    free(code);
    return CDYAR_INVALID_INPUT;
  }

  // make sure typesize if a valid size
  if (typesize == 0) {
    free(code);
    return CDYAR_INVALID_INPUT;
  }

  // make sure there is no overflow
  if (capacity > SIZE_MAX / typesize) {
    free(code);
    return CDYAR_SIZE_T_OVERFLOW;
  }

  // make sure the flags are valid
  cdyar_bool flags_valid;
  areFlagsValid(flags, &flags_valid, code);
  cdyar_returncode tempcode = *code;
  if (*code != CDYAR_SUCCESSFUL) {
    // an issue occured in areFlagsValid, propagate the error upwards
    free(code);
    return tempcode;
  }

  if (!flags_valid) {
    free(code);
    return CDYAR_INVALID_INPUT;
  }

  // make sure handler is not null
  if (!handler) {
    free(code);
    return CDYAR_INVALID_INPUT;
  }

  // allocate memory for the new elements array inside the dynamic array
  // structure
  outptr->elements = malloc(capacity * typesize);
  if (!outptr->elements) {
    free(code);
    return CDYAR_MEMORY_ERROR;
  }

  // zero out all the elements inside the inner array
  memset(outptr->elements, 0, capacity * typesize);

  // set properties
  outptr->capacity = capacity;
  outptr->typesize = typesize;
  outptr->flags = flags;
  outptr->length = 0;
  outptr->code = code;

  // assign resize policy
  if (policy == CDYAR_DEFAULT_RESIZE_POLICY) {
    outptr->policy = cdyar_default_resize_policy;
  } else {
    outptr->policy = policy;
  }

  // assign type handler, indicate success
  outptr->handler = handler;
  return CDYAR_SUCCESSFUL;
}

/*
    free allocated memory for a dynamic array
    args: 1) cdyar_darray* arr     : the dynamic array in question
          2) cdyar_returncode* code: a pointer to a returncode variable to
   report any error (if any) returns: void
*/


cdyar_returncode cdyar_darr(cdyar_darray *arr) {
  // make sure arr is not null
  if (!arr) {
    return CDYAR_DYNAMIC_ARR_DOES_NOT_EXIST;
  }

  CDYAR_CHECK_CODE(arr->code);

  // if the inner array exists, free it
  if (arr->elements) {
    free(arr->elements);
  } else {
    *arr->code = CDYAR_CORRUPTED_DYNAMIC_ARR;
  }

  // free code
  cdyar_returncode tempcode = *arr->code;
  free(arr->code);

  return tempcode;
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
cdyar_returncode cdyar_set(cdyar_darray *arr, const size_t index,
                           void *valueptr) {

  // check that arr is not null
  if (!arr) {
    return CDYAR_DYNAMIC_ARR_DOES_NOT_EXIST;
  }

  // check that code is not null
  CDYAR_CHECK_CODE(arr->code);

  // check that valueptr is not null
  if (!valueptr) {
    *arr->code = CDYAR_INVALID_INPUT;
    return CDYAR_INVALID_INPUT;
  }

  // check that typesize is not zero
  if (arr->typesize == 0) {
    *arr->code = CDYAR_CORRUPTED_DYNAMIC_ARR;
    return CDYAR_CORRUPTED_DYNAMIC_ARR;
  }

  // check for typesize overflow
  if (index > SIZE_MAX / arr->typesize) {
    *arr->code = CDYAR_CORRUPTED_DYNAMIC_ARR;
    return CDYAR_CORRUPTED_DYNAMIC_ARR;
  }

  // check that an elements array actually exists within the dynamic array
  if (!arr->elements) {
    *arr->code = CDYAR_CORRUPTED_DYNAMIC_ARR;
    return CDYAR_CORRUPTED_DYNAMIC_ARR;
  }

  // make sure a resize policy for the dynamic array exists
  if (!arr->policy) {
    *arr->code = CDYAR_CORRUPTED_DYNAMIC_ARR;
    return CDYAR_CORRUPTED_DYNAMIC_ARR;
  }

  // make sure a type handler for the dynamic array exists
  if (!arr->handler) {
    *arr->code = CDYAR_CORRUPTED_DYNAMIC_ARR;
    return CDYAR_CORRUPTED_DYNAMIC_ARR;
  }

  // bounds checking
  if (index >= arr->capacity) {
    *arr->code = CDYAR_ARR_OUT_OF_BOUNDS;

    // return if the automatic resizing is not allowed
    if ((arr->flags & CDYAR_ARR_AUTO_RESIZE) == 0) {
      return CDYAR_ARR_OUT_OF_BOUNDS;
    }
    // invoke the dynamic array's resize policy
    arr->policy(arr, index, arr->code);
  }

  // perform set operation if index is within bounds or a if the array was
  // successfully resized
  if (*arr->code == CDYAR_SUCCESSFUL) {
    // use a typehandler to perform this type of operation operation
    //((CUSTOM_TYPE*)arr->elements)[index] = *((CUSTOM_TYPE*)valueptr);
    // void ptr casted to char* to suppress compiler warnings
    arr->handler(((char *)(arr->elements)) + (arr->typesize * index), valueptr,
                 CDYAR_DIRECTION_ASSIGN_RIGHT_TO_LEFT, arr->code);
    arr->length += 1;
  }

  return *arr->code;
}

cdyar_returncode cdyar_get(const cdyar_darray *arr, const size_t index,
                           void *outptr) {
  // check arr is not null
  if (!arr) {
    return CDYAR_DYNAMIC_ARR_DOES_NOT_EXIST;
  }

  // check code is not null
  CDYAR_CHECK_CODE(arr->code);

  // check *outptr is not null
  if (!outptr) {
    *arr->code = CDYAR_INVALID_INPUT;
    return CDYAR_INVALID_INPUT;
  }

  // check that the typesize is not zero
  if (arr->typesize == 0) {
    *arr->code = CDYAR_CORRUPTED_DYNAMIC_ARR;
    return CDYAR_CORRUPTED_DYNAMIC_ARR;
  }

  // chcek for typesize overflow
  if (index > SIZE_MAX / arr->typesize) {
    *arr->code = CDYAR_CORRUPTED_DYNAMIC_ARR;
    return CDYAR_CORRUPTED_DYNAMIC_ARR;
  }

  // check that an elements array actually exists within the static array
  if (!arr->elements) {
    *arr->code = CDYAR_CORRUPTED_DYNAMIC_ARR;
    return CDYAR_CORRUPTED_DYNAMIC_ARR;
  }

  // check that there is a handler function in the dynamic array
  if (!arr->handler) {
    *arr->code = CDYAR_CORRUPTED_DYNAMIC_ARR;
    return CDYAR_CORRUPTED_DYNAMIC_ARR;
  }

  // bounds checking
  if (index >= arr->capacity) {
    *arr->code = CDYAR_ARR_OUT_OF_BOUNDS;
    return CDYAR_ARR_OUT_OF_BOUNDS;
  }

  // assign outptr to a pointer to the element in question in the array
  // void ptr converted to char* to suppress compiler warnings
  arr->handler(((char *)(arr->elements)) + (arr->typesize * index), outptr,
               CDYAR_DIRECTION_ASSIGN_LEFT_TO_RIGHT, arr->code);

  /**code=CDYAR_SUCCESSFUL*/ // uneccessary since handler will determine code
                             // anyways + it could hide handler failure`
  return *arr->code;
}

cdyar_returncode cdyar_setflags(cdyar_darray *arr, const cdyar_flag flags) {

  // check arr is not null
  if (!arr) {
    return CDYAR_DYNAMIC_ARR_DOES_NOT_EXIST;
  }

  // check code is not null
  CDYAR_CHECK_CODE(arr->code);

  // make sure the flags are valid
  cdyar_bool flags_valid;
  areFlagsValid(flags, &flags_valid, arr->code);
  if (*arr->code != CDYAR_SUCCESSFUL) {
    // an issue occurred in areFlagsValid, propagte the error upwards
    return *arr->code;
  }

  if (!flags_valid) {
    *arr->code = CDYAR_INVALID_INPUT;
    return CDYAR_INVALID_INPUT;
  }

  // set arr->flags to the new value and indicate success
  arr->flags = flags;
  *arr->code = CDYAR_SUCCESSFUL;
  return CDYAR_SUCCESSFUL;
}

cdyar_returncode cdyar_setpolicy(cdyar_darray *arr,
                                 const cdyar_resizepolicy policy) {

  // check that arr is not null
  if (!arr) {
    return CDYAR_DYNAMIC_ARR_DOES_NOT_EXIST;
  }

  // check that code is not null
  CDYAR_CHECK_CODE(arr->code);

  // check that policy is not null
  if (!policy) {
    *arr->code = CDYAR_INVALID_INPUT;
    return CDYAR_INVALID_INPUT;
  }

  // assign new policy and indicate success
  arr->policy = policy;

  *arr->code = CDYAR_SUCCESSFUL;
  return CDYAR_SUCCESSFUL;
}
