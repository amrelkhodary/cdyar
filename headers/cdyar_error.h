#ifndef H_CDYAR_ERROR
    #define H_CDYAR_ERROR

    #include <stdio.h> //for fprintf in CDYAR_CHECK_CODE

    //checking code is not null
    #define CDYAR_CHECK_CODE(code) \
        do { \
            if (!(code)) { \
                fprintf(stderr, "FATAL: NULL error code pointer passed to %s\n", __func__); \
                abort(); \
            } \
        } while(0)

    //simple returncode type
    typedef unsigned int cdyar_returncode;

    enum cdyar_errors {
        CDYAR_SUCCESSFUL,
        CDYAR_FAILED, //generic failed return code, use it only when abs. neccessary
        CDYAR_MEMORY_ERROR,
        CDYAR_INVALID_INPUT,
        CDYAR_ARR_OUT_OF_BOUNDS,

    };

#endif
