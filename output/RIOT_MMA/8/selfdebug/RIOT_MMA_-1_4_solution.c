#include <stdio.h>
#include "periph/i2c.h"
#include "mma8x5x.h"
#include "ztimer.h"

#define I2C_ADDR    CONFIG_MMA8X5X_I2C_ADDRESS  // Correct I2C address macro
#define I2C_DEV     0   // Assuming I2C device 0, adjust as necessary
#define RATE        MMA8X5X_RATE_50HZ
#define RANGE       MMA8X5X_RANGE_2G

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(I2C_DEV),
    .addr = I2C_ADDR,
    .rate = RATE,
    .range = RANGE
};

static void read_sensor(void) {
    mma8x5x_data_t data;
    if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
        printf("X: %d Y: %d Z: %d\n", data.offset[0], data.offset[1], data.offset[2]);
    }
    else {
        printf("Error reading sensor\n");
    }
}

int main(void) {
    // Initialize I2C for the device
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Initialization failed\n");
        return 1;
    }

    // Periodically read the sensor every 5 seconds
    while (1) {
        read_sensor();
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return 0;
}