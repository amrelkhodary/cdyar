#include "../headers/cdyar_arithmetic.h"

void check_sizet_overflow(const size_t count, cdyar_returncode* code, ...) {
    //check code is not null
    CDYAR_CHECK_CODE(code);

    //check that count doesn't equal zero
    if(count == 0) {
        *code=CDYAR_INVALID_INPUT;
        return;
    }

    //macros required for variadic func to work
    va_list args;
    va_start(args, code);

    //keep multiplying until overflow is found
    size_t temp_product = 1;
    size_t left = count;
    size_t next;
    while(left != 0) {
       //get the next number to multiply, if it is zero, return (because there would be no overflow)
       next = va_arg(args, size_t);
       if(next == 0) {
          va_end(args);
          *code=CDYAR_SUCCESSFUL;
          return;
       }

       //check for overflow
       if(temp_product > (SIZE_MAX / next) ) {
          va_end(args);
          *code=CDYAR_SIZE_T_OVERFLOW;
          return;
       }

       //update loop variables
       temp_product *= next;
       left--;
    }

    //va_end is a required macro
    //indicate success
    va_end(args);
    *code=CDYAR_SUCCESSFUL;
}

void cdyar_check_uint_overflow(const size_t count, cdyar_returncode* code, ...) {
    //check code is not null
    CDYAR_CHECK_CODE(code);

    //check that count doesn't equal zero
    if(count == 0) {
        *code=CDYAR_INVALID_INPUT;
        return;
    }

    //macros required for variadic func to work
    va_list args;
    va_start(args, code);

    //keep multiplying until overflow is found
    unsigned int temp_product = 1;
    size_t left = count;
    unsigned int next;
    while(left != 0) {
       //get the next number to multiply, if it is zero, return (because there would be no overflow)
       next = va_arg(args, size_t);
       if(next == 0) {
          va_end(args);
          *code=CDYAR_SUCCESSFUL;
          return;
       }

       //check for overflow
       if(temp_product > (UINT_MAX / next) ) {
          va_end(args);
          *code=CDYAR_SIZE_T_OVERFLOW;
          return;
       }

       //update loop variables
       temp_product *= next;
       left--;
    }

    //va_end is a required macro
    //indicate success
    va_end(args);
    *code=CDYAR_SUCCESSFUL;
}

void cdyar_uintpow(const unsigned int base, const unsigned int exponent, unsigned int* outptr, cdyar_returncode* code) {
    //check that code is not null
    CDYAR_CHECK_CODE(code);

    //check that outptr is not null
    if(!outptr) {
        *code = CDYAR_INVALID_INPUT;
        return;
    }

    //check for 0^0 case
    if(base == 0 && exponent == 0) {
        *code = CDYAR_ARITHMETIC_NOT_A_NUMBER;
        return;
    }

    //start exponentiation
    unsigned int result = 1;
    for(unsigned int i = 0; i<exponent; i++) { //added unsigned int for i to suppress annoying compiler warnings
        cdyar_check_uint_overflow(2, code, result, base);
        if(*code == CDYAR_SUCCESSFUL) {
            //no overflow, multiply
            result *= base;
        } else {
            //overflow or other potential error, propagate the error upwards
            return;
        }
    }

    //assign result to outptr, indicate success (even though it is technically not required here because
    // code will be set to CDYAR_SUCCESSFUL already when if it reaches this point), but it is done for clarity
    // nonetheless
    *outptr = result;
    *code = CDYAR_SUCCESSFUL;
}
