#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <errno.h>
#include <linux/i2c-dev.h>

#include "common.h"

int _write_i2c_data_(int file, unsigned char *writeData, size_t length) {
  if (write(file, writeData, length) != length) {
      perror("Failed to write to i2c bus!\n");
      return -1;
  }
  return 0;
}

int _read_i2c_data_(int file, unsigned char *readData, size_t length) {
  if (read(file, readData, length) != length) {
      perror("Failed to read from the i2c bus!\n");
      return -1;
  }
  return 0;
}

int init_serial(int *fd, const char *dev) {
    struct termios opt;

    /* open serial device */
    if ((*fd = open(dev, O_RDWR)) < 0) {
        perror("open()");
        return -1;
    }

    /* define termois */
    if (tcgetattr(*fd, &opt) < 0) {
        perror("tcgetattr()");
        return -1;
    }

    opt.c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG);
    opt.c_oflag  &= ~OPOST;

    /* Character length, make sure to screen out this bit before setting the data bit */
    opt.c_cflag &= ~CSIZE;

    /* No hardware flow control */
    opt.c_cflag &= ~CRTSCTS;

    /* 8-bit data length */
    opt.c_cflag |= CS8;

    /* 1-bit stop bit */
    opt.c_cflag &= ~CSTOPB;

    /* No parity bit */
    opt.c_iflag |= IGNPAR;

    /* Output mode */
    opt.c_oflag = 0;
    
    /* No active terminal mode */
    opt.c_lflag = 0;

    /* Input baud rate */
    if (cfsetispeed(&opt, B9600) < 0)
        return -1;

    /* Output baud rate */
    if (cfsetospeed(&opt, B9600) < 0)
        return -1;

    /* Overflow data can be received, but not read */
    if (tcflush(*fd, TCIFLUSH) < 0)
        return -1;

    if (tcsetattr(*fd, TCSANOW, &opt) < 0)
        return -1;

    return 0;
}

int serial_write(int *fd, const char *data, size_t size) {
    int ret = write(*fd, data, size);
    if ( ret < 0 ) {
        perror("write");
        tcflush(*fd, TCOFLUSH);
    }

    return ret;
}

int serial_read(int *fd, char *data, size_t size) {
    size_t read_left = size;
    size_t read_size = 0;
    char *read_ptr = data;
    struct timeval timeout = {5, 0};

    memset(data, 0, size);

    fd_set rfds;
    while (1) {
        FD_ZERO(&rfds);
        FD_SET(*fd, &rfds);
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        if (read_left == 0)
            break;

        switch (select(*fd+1, &rfds, NULL, NULL, &timeout)) {
        case -1:
            perror("select()");
            break;
        case 0:
            perror("timeout and retry");
            break;
        default:
            if (FD_ISSET(*fd,&rfds)) {
                read_size = read(*fd, read_ptr, read_left);
                if (read_size == 0)
                    break;

                read_ptr += read_size;
                read_left -= read_size;
            }
        }
    }

    return read_size;
}


