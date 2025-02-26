#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <errno.h>

#include "../common.h"
#include "jxo2101.h"

float parse_o2_data(uint8_t* data) {
    int o2_raw = 0; 
    float o2_value = 0;
    o2_raw = (data[3] << 8) | data[4]; // 高8位和低8位拼接成O2浓度
    o2_value = o2_raw * 0.1;
    return o2_value;
}

int JXO2101_Inquiry(int fd)
{
  uint8_t command[] = {0x01, 0x03, 0x00, 0x02, 0x00, 0x01, 0x25, 0xCA};

  if (serial_write(&fd, command, sizeof(command)) < 0) {
    DEBUG("uart send data failed!\n");
    return -1;
  }
  return 0;
}

int JXO2101_ReadO2Value(int fd)
{
  int len;
  float o2_value = 0;
  uint8_t buffer[9];

  len = serial_read(&fd, buffer, sizeof(buffer));
  if (len > 0) {
    o2_value = parse_o2_data(buffer);
    JXO2101_PRT("Oxygen Concentration: %.1f\n", o2_value);
  } else {
    DEBUG("receive data error! len :%d\n", len);
    return -1;
  }

  return 0;
}

int JXO2101_Run()
{
  int fd;

  init_serial(&fd, UART_DEVICE);
  JXO2101_PRT("JXO2101 uart init success.\n");

  if (JXO2101_Inquiry(fd) < 0) {
    JXO2101_PRT("JXO2101_Inquiry Failed!\n");
    return -1;
  }
  JXO2101_PRT("JXO2101_Inquiry success.\n");

  if (JXO2101_ReadO2Value(fd) < 0) {
    JXO2101_PRT("JXO2101 Read O2 Value Failed!\n");
    close(fd);
    return 1;
  }

  close(fd);
  
  return 0;
}

int main() 
{
  int ret = 0;

  JXO2101_PRT("JXO2101 Run Once.\n");
  ret = JXO2101_Run();
  if (ret != 0) {
    JXO2101_PRT("JXO2101 Run Failed!\n");
    return -1;
  }

  JXO2101_PRT("JXO2101 Run Once success.\n");
  
  return 0;
}