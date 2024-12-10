#include "xtimer.h"
#include "periph/i2c.h"

// Initialize MMA8x5x accelerometer parameters
#define ADDR MMA8X5X_I2C_ADDRESS
#define RATE MMA8X5X_RATE_50HZ
#define RANGE MMA8X5X_RANGE_2G

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = ADDR,
    .rate = RATE,
    .range = RANGE
};

void read_accelerometer(void) {
    mma8x5x_data_t data;
    int res = mma8x5x_init(&dev, &params);
    if (res == MMA8X5X_OK) {
        res = mma8x5x_read(&dev, &data);
        if (res == MMA8X5X_OK) {
            // Print data assuming correct members
            printf("Acceleration in X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        }
        else {
            printf("Error reading accelerometer data\n");
        }
    }
    else {
        printf("Error initializing accelerometer\n");
    }
}

int main(void) {
    xtimer_sleep(5); // Wait for 5 seconds before reading sensor
    while (1) {
        read_accelerometer();
        xtimer_sleep(5); // Read every 5 seconds
    }
    return 0;
}