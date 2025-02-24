#ifndef _MHZ19D_H_
#define _MHZ19D_H_

#ifndef MHZ19D_PRT
#define MHZ19D_PRT(fmt...)   \
do {\
    printf("[MHZ19D_PRT][%s][%5d] ", __FUNCTION__, __LINE__);\
    printf(fmt);\
}while(0)
#endif

#endif