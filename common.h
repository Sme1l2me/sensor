#ifndef __COMMON_H_
#define __COMMON_H_

int _write_i2c_data_(int file, unsigned char *writeData, size_t length);
int _read_i2c_data_(int file, unsigned char *readData, size_t length);

#endif