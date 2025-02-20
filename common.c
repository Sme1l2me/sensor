#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
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



