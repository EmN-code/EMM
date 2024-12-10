#include <stdio.h>  // Correctly include for printf
#include <periph/i2c.h>

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,  // Ensure the correct address macro
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }

    while (1) {
        if (mma8x5x_read(&dev, &data) != MMA8X5X_OK) {
            printf("Failed to read data from MMA8x5x\n");
        } else {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        }
        xtimer_sleep(5);  // Sleep for 5 seconds
    }
    return 0;
}
