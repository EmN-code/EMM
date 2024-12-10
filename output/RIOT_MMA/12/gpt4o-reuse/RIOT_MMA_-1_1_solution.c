
#include <stdio.h>
#include <xtimer.h>
#include <mma8x5x.h>

#define MMA8X5X_I2C_ADDRESS (0x1D)
#define MMA8X5X_I2C_DEV     (I2C_DEV(0))
#define MMA8X5X_ACC_RANGE   (MMA8X5X_RANGE_2G)

mma8x5x_t dev;
mma8x5x_params_t params = {
    .i2c = MMA8X5X_I2C_DEV,
    .addr = MMA8X5X_I2C_ADDRESS,
    .range = MMA8X5X_ACC_RANGE,
};

int main(void) {
    int16_t x, y, z;

    // Initialize the MMA8x5x sensor
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization of MMA8x5x sensor failed");
        return 1;
    }

    while (1) {
        // Read acceleration data
        mma8x5x_read(&dev, &x, &y, &z);

        // Print acceleration data
        printf("X: %d, Y: %d, Z: %d\n", x, y, z);

        // Delay for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}
