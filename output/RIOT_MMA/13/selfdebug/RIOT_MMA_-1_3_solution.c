#include <stdio.h>
#include <stdlib.h>
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "periph_cpu.h"
#include "mma8x5x.h"

#define I2C_DEVICE      I2C_DEV(0)  // Assuming I2C_DEV(0) is the correct I2C device
#define MMA8X5X_ADDRESS CONFIG_MMA8X5X_I2C_ADDRESS
#define TIMER_SECONDS   5  // 5-second interval

static mma8x5x_t dev;
static mma8x5x_data_t data;

void timer_callback(void *arg) {
    (void)arg;
    mma8x5x_read(&dev, &data);
    printf("X: %d Y: %d Z: %d\n", data.x, data.y, data.z);
    timer_set(TIMER_SECONDS * 1000000, timer_callback, NULL);
}

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEVICE,
        .addr = MMA8X5X_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }

    // Setup a timer to call timer_callback every 5 seconds
    timer_set(TIMER_SECONDS * 1000000, timer_callback, NULL);

    // Keep the process running
    while (1) {
        // Do nothing, wait for timer interrupts
    }

    return 0;
}