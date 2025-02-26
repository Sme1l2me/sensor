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

// int _uart_init_(int file);
// int _uart_send_data_(int uart_fd, uint8_t* data, size_t len);
// int _uart_receive_data_(int uart_fd, uint8_t* buffer, size_t len);

#endif
