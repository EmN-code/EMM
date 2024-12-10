#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include "mma8x5x.h"
#include "periph/i2c.h"

#define INTERVAL (5U * US_PER_SEC)  // 5 seconds

static mma8x5x_t dev;

int main(void) {
    // Define parameters
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    // Initialize MMA8x5x sensor
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x!\n");
        return 1;
    }
    printf("MMA8x5x initialized successfully!\n");

    // Prepare data structure
    mma8x5x_data_t data;

    while (1) {
      // Read sensor data
      if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
          printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
      } else {
          printf("Failed to read from MMA8x5x\n");
      }
      // Wait for the next measurement interval
      xtimer_sleep(INTERVAL);
    }
    return 0;
}