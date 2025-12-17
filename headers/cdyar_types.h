#ifndef H_CDYAR_TYPES
#define H_CDYAR_TYPES
#include "./cdyar_structures.h"

//directions for cdyar_typehandler function
enum cdyar_directions {
   CDYAR_DIRECTION_ASSIGN_LEFT_TO_RIGHT,
   CDYAR_DIRECTION_ASSIGN_RIGHT_TO_LEFT,
};

//typehandler function type
//casts both pointers to a specific type, assigns one to the other
//depending on direction passed
typedef void (*cdyar_typehandler)(void *left_voidptr, void *right_voidptr,
                                  cdyar_flag direction, cdyar_returncode* code);
#endif
