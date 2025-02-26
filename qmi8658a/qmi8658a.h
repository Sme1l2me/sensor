#ifndef _QMI8658A_H_
#define _QMI8658A_H_

#ifndef QMI8658A_PRT
#define QMI8658A_PRT(fmt...)   \
do {\
    printf("[QMI8658A_PRT][%s][%5d] ", __FUNCTION__, __LINE__);\
    printf(fmt);\
}while(0)
#endif

#define I2C_DEVICE            "/dev/i2c-2"
#define QMI8658A_SLAVE_ADDRESS   0x6a

#define AX_H    0x36
#define AY_H    0x37
#define AZ_H    0x38
#define GX_H    0x3c
#define GY_H    0x3d
#define GZ_H    0x3e

#define STATUS  0x2d

#define CTRL1   0x02    //serial interface and sensor enable
#define CTRL2   0x03    //accelerometer setting
#define CTRL3   0x04    //gyroscope setting
#define CTRL5   0x06    //sensor data processing setting
#define CTRL7   0x08    //enable sensor and configure data reads
#define CTRL8   0x09    //motion detection control

#define RESET   0x60

int QMI8658A_SoftReset(int file);
int QMI8658A_ReadAcceleration(int file, float *ax, float *ay, float *az);
int QMI8658A_ReadAngularRate(int file, float *gx, float *gy, float *gz);
int QMI8658A_Run();

#endif