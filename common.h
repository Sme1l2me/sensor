#ifndef __COMMON_H_
#define __COMMON_H_

#ifndef DEBUG
#define DEBUG(fmt...)   \
do {\
    printf("[DEBUG][%s][%5d] ", __FUNCTION__, __LINE__);\
    printf(fmt);\
}while(0)
#endif

int _write_i2c_data_(int file, unsigned char *writeData, size_t length);
int _read_i2c_data_(int file, unsigned char *readData, size_t length);

int init_serial(int *fd, const char *dev);
int serial_write(int *fd, const char *data, size_t size);
int serial_read(int *fd, char *data, size_t size);

#endif
