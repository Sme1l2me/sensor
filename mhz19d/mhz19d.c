#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <errno.h>

#include "mhz19d.h"
#include "../common.h"

int parse_co2_data(uint8_t* data) {
    int co2_ppm = (data[2] << 8) | data[3]; // 高8位和低8位拼接成CO2浓度
    return co2_ppm;
}

char get_check_sum(char *packet) {
  char i, checksum;
  for(i = 1; i < 8; i++) {
    checksum += packet[i]; 
  }

  checksum = 0xff - checksum;
  checksum += 1;
  return checksum;
}

//5000pm
int MHZ19D_SetRange(int fd)
{
  uint8_t command[] = {0xFF, 0x01, 0x99, 0x00, 0x00, 0x00, 0x13, 0x88, 0xCB};

  if (serial_write(&fd, command, sizeof(command)) < 0) {
    DEBUG("uart send data failed!\n");
    return -1;
  }

  printf("Command: ");
    for (size_t i = 0; i < 9; i++) {
        printf("%02X ", command[i]);
    }
    printf("\n");

  return 0;
}

int MHZ19D_SendCommand(int fd)
{
  int ret;
  uint8_t command[] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};

  ret = serial_write(&fd, command, sizeof(command));
  if (ret < 0) {
    DEBUG("uart send data failed!\n");
    return -1;
  }
  printf("ret: %d\n", ret);
  
  printf("Command: ");
    for (size_t i = 0; i < 9; i++) {
        printf("%02X ", command[i]);
    }
  printf("\n");

  return 0;
}

int MHZ19D_ReadCo2Value(int fd)
{
  int len;
  int co2_ppm = 0;
  uint8_t buffer[9];

  len = serial_read(&fd, buffer, sizeof(buffer));
  printf("receive data: ");
    for (size_t i = 0; i < len; i++) {
        printf("%02X ", buffer[i]);
    }
  printf("\n");

  if (len > 0) {
    co2_ppm = parse_co2_data(buffer);
    MHZ19D_PRT("CO2 Concentration: %dppm\n", co2_ppm);
  } else {
    DEBUG("receive data error! len :%d\n", len);
    return -1;
  }

  return 0;
}

int MHZ19D_Run()
{
  int fd;

  init_serial(&fd, UART_DEVICE);
  MHZ19D_PRT("MHZ19D uart init success.\n");

  if (MHZ19D_SetRange(fd) < 0) {
    MHZ19D_PRT("MHZ19D Set Range Failed!\n");
    close(fd);
    return 1;
  }
  MHZ19D_PRT("MHZ19D Set Range success.\n");

  if (MHZ19D_SendCommand(fd) < 0) {
    MHZ19D_PRT("MHZ19D Send Command Failed!\n");
    close(fd);
    return 1;
  }
  MHZ19D_PRT("MHZ19D Send Command success.\n");

  if (MHZ19D_ReadCo2Value(fd) < 0) {
    MHZ19D_PRT("MHZ19D Read CO2 Value Failed!\n");
    close(fd);
    return 1;
  }

  close(fd);

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
