#include <stdio.h>
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

int _uart_init_(int file) {
  int uart_fd = open(file, O_RDWR | O_NOCTTY | O_SYNC);
    if (uart_fd == -1) {
        perror("Error opening UART");
        return -1;
    }

    struct termios tty;
    if (tcgetattr(uart_fd, &tty) != 0) {
        perror("Error getting UART attributes");
        close(uart_fd);
        return -1;
    }

    cfsetospeed(&tty, B9600);  // 设置波特率为9600
    cfsetispeed(&tty, B9600);

    tty.c_cflag &= ~PARENB;    // 无校验位
    tty.c_cflag &= ~CSTOPB;    // 1位停止位
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;        // 8位数据位
    tty.c_cflag &= ~CRTSCTS;   // 不使用硬件流控制
    tty.c_cflag |= CREAD | CLOCAL; // 开启接收器并忽略控制线

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // 禁用软件流控制
    tty.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);  // 禁用输入处理

    tty.c_oflag &= ~OPOST; // 禁用输出处理

    // 设置新的串口属性
    if (tcsetattr(uart_fd, TCSANOW, &tty) != 0) {
        perror("Error setting UART attributes");
        close(uart_fd);
        return -1;
    }

    return uart_fd;
}

int _uart_send_data_(int uart_fd, uint8_t* data, size_t len) {
    if (write(uart_fd, data, len) != len) {
        perror("Error sending data");
        return -1;
    }

    return 0;
}

int _uart_receive_data_(int uart_fd, uint8_t* buffer, size_t len) {
    ssize_t n = read(uart_fd, buffer, len);
    if (n < 0) {
        perror("Error reading data");
        return -1;
    }
    return n;
}
