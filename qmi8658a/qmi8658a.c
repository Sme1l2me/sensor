#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#include "../common.h"
#include "qmi8658a.h"

int QMI8568A_ConfigMode(int file) 
{
  uint8_t command[2] = {CTRL2, 0x00};

  if(_write_i2c_data_(file, command, sizeof(command)) < 0) {
    return -1;
  }
  return 0;
}

int QMI8568A_Enable(int file)
{
  uint8_t command[2] = {CTRL7, 0x83};

  if(_write_i2c_data_(file, command, sizeof(command)) < 0) {
    return -1;
  }
  return 0;
}

int QMI8658A_GetStatus(int file)
{
  uint8_t command[1] = {STATUS};
  uint8_t buffer[1];

  if(_write_i2c_data_(file, command, sizeof(command)) < 0) {
    return -1;
  }
  
  while(1) {
    if (_read_i2c_data_(file, buffer, sizeof(buffer)) < 0) {
      return -1;
    }
    if ((buffer[0] & 1 ) & (((buffer[0] >> 1) & 1) == 0))
      QMI8658A_PRT("QMI8658A ready to read.\n");
      break;
  }

  return 0;
}

//aODR 1000hz aFS ±16g
int QMI8658A_ConfigAcceleration(int file)
{
  uint8_t command[2] = {CTRL2, 0x33};

  if(_write_i2c_data_(file, command, sizeof(command)) < 0) {
    return -1;
  }
  return 0;
}

int QMI8658A_ReadAcceleration(int file, float *ax, float *ay, float *az) 
{
  uint8_t command[1];
  uint8_t buffer[2];
  int16_t ax_raw, ay_raw, az_raw;

  if (QMI8658A_ConfigAcceleration(file) < 0) {
    QMI8658A_PRT("QMI8658A Config Acceleration Failed!\n");
  }

  command[0] = AX_H;
  if(_write_i2c_data_(file, command, sizeof(command)) < 0) {
    return -1;
  }
  if (_read_i2c_data_(file, buffer, sizeof(buffer)) < 0) {
    return -1;
  }
  ax_raw = (buffer[0] << 8) | buffer[1];

  command[0] = AY_H;
  if(_write_i2c_data_(file, command, sizeof(command)) < 0) {
    return -1;
  }
  if (_read_i2c_data_(file, buffer, sizeof(buffer)) < 0) {
    return -1;
  }
  ay_raw = (buffer[0] << 8) | buffer[1];

  command[0] = AZ_H;
  if(_write_i2c_data_(file, command, sizeof(command)) < 0) {
    return -1;
  }
  if (_read_i2c_data_(file, buffer, sizeof(buffer)) < 0) {
    return -1;
  }
  az_raw = (buffer[0] << 8) | buffer[1];

  *ax = (float)ax_raw / 2048;   //g = raw_data / (2^15 / aFS)
  *ay = (float)ay_raw / 2048; 
  *az = (float)az_raw / 2048;
  
  return 0;
}

//gODR 1793.6hz gFS ±2048g
int QMI8658A_ConfigAngularRate(int file)
{
  uint8_t command[2] = {CTRL3, 0x72};

  if(_write_i2c_data_(file, command, sizeof(command)) < 0) {
    return -1;
  }
  return 0;
}

int QMI8658A_ReadAngularRate(int file, float *gx, float *gy, float *gz) 
{
  uint8_t command[1];
  uint8_t buffer[2];
  int16_t gx_raw, gy_raw, gz_raw;

  if (QMI8658A_ConfigAngularRate(file) < 0) {
    QMI8658A_PRT("QMI8658A Config AngularRate Failed!\n");
  }

  command[0] = GX_H;
  if(_write_i2c_data_(file, command, sizeof(command)) < 0) {
    return -1;
  }
  if (_read_i2c_data_(file, buffer, sizeof(buffer)) < 0) {
    return -1;
  }
  gx_raw = (buffer[0] << 8) | buffer[1];

  command[0] = GY_H;
  if(_write_i2c_data_(file, command, sizeof(command)) < 0) {
    return -1;
  }
  if (_read_i2c_data_(file, buffer, sizeof(buffer)) < 0) {
    return -1;
  }
  gy_raw = (buffer[0] << 8) | buffer[1];

  command[0] = GZ_H;
  if(_write_i2c_data_(file, command, sizeof(command)) < 0) {
    return -1;
  }
  if (_read_i2c_data_(file, buffer, sizeof(buffer)) < 0) {
    return -1;
  }
  gz_raw = (buffer[0] << 8) | buffer[1];

  *gx = (float)gx_raw / 16;   //dps = raw_data / (2^15 / gFS)
  *gy = (float)gy_raw / 16;
  *gz = (float)gz_raw / 16;
  
  return 0;
}

int QMI8658A_Run() 
{
  int file;
  float ax, ay, az;
  float gx, gy, gz;

  //打开i2c设备
  file = open(I2C_DEVICE, O_RDWR);
  if (file < 0) {
    perror("Failed to open the i2c bus");
    close(file);
    return 1;
  }

  //设置设备地址
  if (ioctl(file, I2C_SLAVE, QMI8658A_SLAVE_ADDRESS) < 0) {
    perror("Failed to set QMI8658A i2c address!");
    close(file);
    return 1;
  }

  //配置模式
  if (QMI8568A_ConfigMode(file) < 0) {
    QMI8658A_PRT("QMI8658A Config Mode Failed!\n");
    close(file);
    return 1;
  }
  QMI8658A_PRT("QMI8658A Config Mode success.\n");

  //使能
  if (QMI8568A_Enable(file) < 0) {
    QMI8658A_PRT("QMI8658A Enable Failed!\n");
    close(file);
    return 1;
  }
  QMI8658A_PRT("QMI8658A Enable success.\n");

  //获取状态
  if (QMI8658A_GetStatus(file) < 0) {
    QMI8658A_PRT("QMI8658A Get Status Failed!\n");
    close(file);
    return 1;
  }

  //读取加速度与角速度
  if (QMI8658A_ReadAcceleration(file, &ax, &ay, &az) < 0) {
    QMI8658A_PRT("QMI8658A Read Acceleration Failed!\n");
    close(file);
    return 1;
  }

  if (QMI8658A_ReadAngularRate(file, &gx, &gy, &gz) < 0) {
    QMI8658A_PRT("QMI8658A Read AngularRate Failed!\n");
    close(file);
    return 1;
  }

  //打印结果
  QMI8658A_PRT("Acceleration: AX=%.2fg, AY=%.2fg, AZ=%.2fg", ax, ay, az);
  QMI8658A_PRT("AngularRate: GX=%.2f°/s, GY=%.2f°/s, GZ=%.2f°/s", gx, gy, gz);

  close(file);

  return 0;
}

int main() 
{
  int ret = 0;

  QMI8658A_PRT("QMI8658A Run Once.\n");
  ret = QMI8658A_Run();
  if(ret != 0) {
    QMI8658A_PRT("QMI8658A Run Failed!\n");
    return -1;
  }

  QMI8658A_PRT("QMI8658A Run Once success.\n");

  return 0;
}
