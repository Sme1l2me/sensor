#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#include "../common.h"
#include "m2313.h"

#define I2C_DEVICE            "/dev/i2c-2"
#define M2313_SLAVE_ADDRESS   0x78
#define M2313_DELAY_US        (40000)     //delay 40ms

int M2313_GetCal(int file)
{
  uint8_t command[2] = {0x78, 0xAC};

  if(_write_i2c_data_(file, command, sizeof(command)) < 0) {
    return -1;
  }
  usleep(M2313_DELAY_US);
  return 0;
}

int M2313_ReadValue(int file, float *pressure, float *temperature)
{
  uint8_t command[1] = {0x78};
  uint8_t buffer[6];
  uint32_t raw_bridge, raw_temperature;
  float bridge;

  if(_write_i2c_data_(file, command, sizeof(command)) < 0) {
    return -1;
  }
  if (_read_i2c_data_(file, buffer, sizeof(buffer)) < 0) {
    return -1;
  }

  printf("Raw Data Read from M2313: ");
  for (int i = 0; i < sizeof(buffer); i++) {
    printf("0x%02X ", buffer[i]);
  }
  printf("\n");

  //解析数据为电桥值和温度值
  raw_bridge = ((uint32_t)buffer[2] << 16) | ((uint32_t)buffer[3] << 8) | (uint32_t)buffer[4];
  raw_temperature = ((uint16_t)buffer[5] << 8) | (uint16_t)buffer[6];

  *temperature = (float)(raw_temperature * 190 / 0x10000) - 40;

  bridge = (float)raw_bridge * 100 / 0x1000000;
  *pressure = (bridge - (0x1000000 * 0.1)) * 8.2;     //Pmax:1100 Pmin:0

  return 0;
}

int M2313_Run() 
{
  int file;
  float pressure, temperature;

  //打开i2c设备
  file = open(I2C_DEVICE, O_RDWR);
  if (file < 0) {
    perror("Failed to open the i2c bus");
    close(file);
    return 1;
  }

  //设置设备地址
  if (ioctl(file, I2C_SLAVE, M2313_SLAVE_ADDRESS) < 0) {
    perror("Failed to set M2313 i2c address!");
    close(file);
    return 1;
  }

  //M2313启动测量
  if (M2313_GetCal(file) < 0) {
    M2313_PRT("M2313 Get Cal Failed!\n");
    close(file);
    return 1;
  }
  M2313_PRT("M2313 Get Cal success.\n");

  //读取电桥数据与温度数据
  if (M2313_ReadValue(file, &pressure, &temperature) < 0) {
    M2313_PRT("M2313 Read Value Failed!\n");
    close(file);
    return 1;
  }

  //打印结果
  M2313_PRT("Read environment data:\n");
  M2313_PRT("Temperature: %.2f°C Pressure: %.2fhPa \n",temperature, pressure);

  close(file);

  return 0;
}

int main() 
{
  int ret = 0;

  M2313_PRT("M2313 Run Once.\n");
  ret = M2313_Run();
  if(ret != 0) {
    M2313_PRT("M2313 Run Failed!\n");
    return -1;
  }

  M2313_PRT("M2313 Run Once success.\n");

  return 0;
}
