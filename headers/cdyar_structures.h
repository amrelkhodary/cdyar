#ifndef H_CDYAR_STRUCTURES
    #define H_CDYAR_STRUCTURES
    //#include "./cdyar_darray.h" //caused a circular dependency error so it had to commented out
                                  //initially it was added to make the darray type available
                                  //will just declare the type here in its place
                                  struct cdyar_darray;
    #include "./cdyar_error.h"

    //simple boolean type
    typedef enum cdyar_bool {
        cdyar_false,
        cdyar_true
    } cdyar_bool;

    //simple flag type
    typedef unsigned int cdyar_flag;
#endif
