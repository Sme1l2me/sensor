#ifndef _M2313_H_
#define _M2313_H_

#ifndef M2313_PRT
#define M2313_PRT(fmt...)   \
do {\
    printf("[M2313_PRT][%s][%5d] ", __FUNCTION__, __LINE__);\
    printf(fmt);\
}while(0)
#endif

#define I2C_DEVICE            "/dev/i2c-2"
#define M2313_SLAVE_ADDRESS   0x78

int M2313_GetCal(int file);
int M2313_ReadValue(int file, float *pressure, float *temperature);
int M2313_Run();

#endif