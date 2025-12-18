#include "../headers/cdyar_types.h"
#include "../headers/cdyar_error.h"

/*
    test code, just to create a template for a typehandler
    function creation macro

    NOT SUPPOSED TO BE USED.
*/
void cdyar_inttypehandler(void *left_ptr, void *right_ptr, cdyar_flag direction, cdyar_returncode *code) {
    //check code is not null
    CDYAR_CHECK_CODE(code);

    //check left_ptr is not null
    if(!left_ptr) {
       *code=CDYAR_INVALID_INPUT;
       return;
    }

    //check right_ptr is not null
    if(!right_ptr) {
        *code=CDYAR_INVALID_INPUT;
        return;
    }

   //cast left, right ptrs to int* type
   int* internal_leftptr = (int*) left_ptr;
   int* internal_rightptr = (int*) right_ptr;

   //assign based on direction
   switch (direction) {
       case CDYAR_DIRECTION_ASSIGN_LEFT_TO_RIGHT:
            *internal_rightptr = *internal_leftptr;
            break;
       case CDYAR_DIRECTION_ASSIGN_RIGHT_TO_LEFT:
            *internal_leftptr = *internal_rightptr;
            break;
       default:
            *code=CDYAR_INVALID_INPUT;
            return;
   }

   *code=CDYAR_SUCCESSFUL;
}
