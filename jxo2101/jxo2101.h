#ifndef _JXO2101_H_
#define _JXO2101_H_

#ifndef JXO2101_PRT
#define JXO2101_PRT(fmt...)   \
do {\
    printf("[JXO2101_PRT][%s][%5d] ", __FUNCTION__, __LINE__);\
    printf(fmt);\
}while(0)
#endif

#define UART_DEVICE   "/dev/ttyS0"

int JXO2101_Inquiry(int fd);
int JXO2101_ReadO2Value(int fd);
int JXO2101_Run();

#endif