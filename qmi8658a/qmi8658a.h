#ifndef _QMI8658A_H_
#define _QMI8658A_H_

#ifndef QMI8658A_PRT
#define QMI8658A_PRT(fmt...)   \
do {\
    printf("[QMI8658A_PRT][%s][%5d] ", __FUNCTION__, __LINE__);\
    printf(fmt);\
}while(0)
#endif

#endif