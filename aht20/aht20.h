#ifndef __AHT20_H_
#define __AHT20_H_

#ifndef AHT20_PRT
#define AHT20_PRT(fmt...)   \
do {\
    printf("[AHT20_PRT][%s][%5d] ", __FUNCTION__, __LINE__);\
    printf(fmt);\
}while(0)
#endif

#define I2C_DEVICE            "/dev/i2c-2"
#define AHT20_SLAVE_ADDRESS   0x38
#define AHT20_DELAY_US        (80000)     //delay 80ms

int AHT20_StartMeasure(int file);
int AHT20_ReadValue(int file, float *humidity, float *temperature);
int AHT20_Run();

#endif