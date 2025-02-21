#ifndef __AHT20_H_
#define __AHT20_H_

#ifndef AHT20_PRT
#define AHT20_PRT(fmt...)   \
do {\
    printf("[AHT20_PRT][%s][%5d] ", __FUNCTION__, __LINE__);\
    printf(fmt);\
}while(0)
#endif


int AHT20_StartMeasure(int file);
int AHT20_ReadValue(int file, float *humidity, float *temperature);
int AHT20_Run();

#endif