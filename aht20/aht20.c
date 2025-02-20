#include "../common.h"
#include "aht20.h"

#define I2C_DEVICE            "/dev/i2c-2"
#define AHT20_SLAVE_ADDRESS   0x70
#define AHT20_DELAY_US        (80000)     //delay 80ms

unsigned char Calc_CRC8(unsigned char *message, unsigned char length) {
  unsigned char i;
  unsigned char byte;
  unsigned char crc = 0xFF;
  for (byte = 0; byte < length; byte++) {
    crc ^= (message[byte]);
    for (i = 8; i > 0; --i) {
      if (crc & 0x80) {
        crc = (crc << 1) ^ 0x31;
      } else {
        crc <<= 1;
      }
    }
  }
  return crc;
}

int AHT20_StartMeasure(int file)
{
  uint8_t command[3] = {0xAC, 0x33, 0x00};

  if(_write_i2c_data_(file, command, sizeof(command)) < 0) {
    return -1;
  }
  usleep(AHT20_DELAY_US);
  return 0;
}

int AHT20_ReadValue(int file, float *humidity, float *temperature)
{
  uint8_t buffer[7];
  uint32_t raw_humidity, raw_temperature;
  uint8_t crc_humidity, crc_temperature;

  if (_read_i2c_data_(file, buffer, sizeof(buffer)) < 0) {
    return -1;
  }

  // 校验CRC
  crc_humidity = Calc_CRC8(buffer, 5);  // 湿度数据CRC校验（前5个字节）
  crc_temperature = Calc_CRC8(buffer + 3, 3);  // 温度数据CRC校验（后3个字节）

  if (crc_humidity != buffer[5]) {
      printf("CRC check failed for humidity data!\n");
      return -1;
  }
  if (crc_temperature != buffer[6]) {
      printf("CRC check failed for temperature data!\n");
      return -1;
  }

    // 将数据解析为湿度和温度值
  raw_humidity = ((uint32_t)buffer[1] << 12) | ((uint32_t)buffer[2] << 4) | ((uint32_t)buffer[3] >> 4);
  raw_temperature = ((uint32_t)(buffer[3] & 0x0F) << 16) | ((uint32_t)buffer[4] << 8) | buffer[5];

  *humidity = (float)raw_humidity * 100.0 / 0x100000;
  *temperature = (float)(raw_temperature * 200.0 / 0x100000) - 50.0;

  return 0;
}

