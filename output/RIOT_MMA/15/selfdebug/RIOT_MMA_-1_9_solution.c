#include <ztimer.h>
#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <mma8x5x.h>

#define I2C_ADDR MMA8X5X_I2C_ADDRESS // Correcting the undeclared error
#define INTERVAL (5 * ZTIMER_MSEC_PER_SEC) // 5-second interval

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = I2C_ADDR, // Use the corrected I2C address macro
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void read_mma8x5x(void) {
    mma8x5x_data_t data;
    if (mma8x5x_read(&dev, &data) == 0) {
        printf("X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
    }
    else {
        printf("Failed to read data from MMA8x5x\n");
    }
}

int main(void) {
    if (mma8x5x_init(&dev, &params) == MMA8X5X_OK) {
        puts("MMA8x5x initialized successfully.");
    } else {
        puts("MMA8x5x initialization failed.");
        return 1;
    }

    while (1) {
        read_mma8x5x();
        ztimer_sleep(ZTIMER_MSEC, INTERVAL); // Wait for the next read
    }

    return 0;
}