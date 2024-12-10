#include "xtimer.h"
#include "stdio.h"
#include "periph/i2c.h"
#include "mma8x5x.h"

#define I2C_DEV          I2C_DEV(0)
#define MMA8X5X_ADDR     (0x1D)  // Default I2C address for MMA8x5x
#define READ_INTERVAL    (5U)    // 5 seconds interval

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV,
    .addr = MMA8X5X_ADDR,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void) {
    int result;
    mma8x5x_data_t data;

    // Initialize the I2C interface
    if (i2c_init(I2C_DEV) != 0) {
        printf("Failed to initialize I2C\n");
        return 1;
    }

    // Initialize the MMA8x5x device
    result = mma8x5x_init(&dev, &params);
    if (result != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x device\n");
        return 1;
    }

    while (1) {
        // Read acceleration data
        result = mma8x5x_read(&dev, &data);
        if (result == MMA8X5X_OK) {
            printf("Acceleration: X=%d, Y=%d, Z=%d\n", data.x, data.y, data.z);
        } else {
            printf("Failed to read acceleration data\n");
        }

        // Wait for the next interval
        xtimer_sleep(READ_INTERVAL);
    }

    return 0;
}