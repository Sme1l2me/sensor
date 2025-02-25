#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <errno.h>

#include "mhz19d.h"

int parse_co2_data(uint8_t* data) {
    int co2_ppm = (data[2] << 8) | data[3]; // 高8位和低8位拼接成CO2浓度
    return co2_ppm;
}

int MHZ19D_ReadCo2Value(int fd)
{
  int len;
  int co2_ppm = 0;
  uint8_t command[] = {};
  uint8_t buffer[9];

  if (_uart_send_data_(fd, command, sizeof(command)) < 0) {
    return -1;
  }

  len = _uart_receive_data_(fd, buffer, sizeof(buffer))
  if (len > 0) {
    co2_ppm = parse_co2_data(buffer);
    MHZ19D_PRT("CO2 Concentration: %dppm\n", co2_ppm);
  } else {
    MHZ19D_PRT("receive data error! len :%d\n", len);
    return -1;
  }

  close(fd);
  return 0;
}

int MHZ19D_Run()
{
  int fd;

  fd = _uart_init_(UART_DEVICE);
  MHZ19D_PRT("MHZ19D uart init success.\n");

  if (MHZ19D_ReadCo2Value(fd) < 0) {
    MHZ19D_PRT("MHZ19D Read CO2 Value Failed!\n");
    close(fd);
    return 1;
  }

  return 0;
}

int main() 
{
  int ret = 0;

  MHZ19D_PRT("MHZ19D Run Once.\n");
  ret = MHZ19D_Run();
  if (ret != 0) {
    MHZ19D_PRT("MHZ19D Run Failed!\n");
    return -1;
  }

  MHZ19D_PRT("MHZ19D Run Once success.\n");
  
  return 0;
}