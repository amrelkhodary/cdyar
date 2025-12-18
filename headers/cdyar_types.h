#ifndef H_CDYAR_TYPES
#define H_CDYAR_TYPES
#include "./cdyar_structures.h"

//macro for creating typehandler functions
#define CDYAR_DEFINE_TYPE_HANDLER(type, suffix) \
void cdyar_##suffix##_typehandler(void *left_ptr, void *right_ptr, \
                                   cdyar_flag direction, cdyar_returncode *code) { \
    /* Check code pointer is valid */ \
    CDYAR_CHECK_CODE(code); \
    \
    /* Validate input pointers */ \
    if (!left_ptr) { \
        *code = CDYAR_INVALID_INPUT; \
        return; \
    } \
    if (!right_ptr) { \
        *code = CDYAR_INVALID_INPUT; \
        return; \
    } \
    \
    /* Check pointer alignment to prevent undefined behavior */ \
    if ((uintptr_t)left_ptr % _Alignof(type) != 0) { \
        *code = CDYAR_INVALID_INPUT; \
        return; \
    } \
    if ((uintptr_t)right_ptr % _Alignof(type) != 0) { \
        *code = CDYAR_INVALID_INPUT; \
        return; \
    } \
    \
    /* Cast pointers - safe after alignment checks */ \
    type *internal_leftptr = (type *)left_ptr; \
    type *internal_rightptr = (type *)right_ptr; \
    \
    /* Perform assignment based on direction */ \
    switch (direction) { \
        case CDYAR_DIRECTION_ASSIGN_LEFT_TO_RIGHT: \
            *internal_rightptr = *internal_leftptr; \
            break; \
        case CDYAR_DIRECTION_ASSIGN_RIGHT_TO_LEFT: \
            *internal_leftptr = *internal_rightptr; \
            break; \
        default: \
            *code = CDYAR_INVALID_INPUT; \
            return; \
    } \
    \
    /* Success */ \
    *code = CDYAR_SUCCESSFUL; \
}


// directions for cdyar_typehandler function
enum cdyar_directions {
  CDYAR_DIRECTION_ASSIGN_LEFT_TO_RIGHT,
  CDYAR_DIRECTION_ASSIGN_RIGHT_TO_LEFT,
};

// typehandler function type
// casts both pointers to a specific type, assigns one to the other
// depending on direction passed
typedef void (*cdyar_typehandler)(void *left_voidptr, void *right_voidptr,
                                  cdyar_flag direction, cdyar_returncode *code);

// defining some basic int type handler
void cdyar_inttypehandler(void *left_ptr, void *right_ptr, cdyar_flag direction,
                          cdyar_returncode *code);
#endif
