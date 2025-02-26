#ifndef _MHZ19D_H_
#define _MHZ19D_H_

#ifndef MHZ19D_PRT
#define MHZ19D_PRT(fmt...)   \
do {\
    printf("[MHZ19D_PRT][%s][%5d] ", __FUNCTION__, __LINE__);\
    printf(fmt);\
}while(0)
#endif

#define UART_DEVICE   "/dev/ttyS0"

int MHZ19D_SendCommand(int fd);
int MHZ19D_ReadCo2Value(int fd);
int MHZ19D_Run();

#endif