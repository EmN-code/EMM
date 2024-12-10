#include stdio.h
#include stdbool.h
#include stdint.h
#include periph/gpio.h
#include xtimer.h
#include i2c_t

// Define the MMA8x5x parameters
mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

mma8x5x_t dev;
mma8x5x_data_t data;

void read_mma8x5x(void) {
    if (mma8x5x_read(&dev, &data) == 0) {
        printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
    } else {
        printf("Failed to read data from MMA8x5x.\n");
    }
}

int main(void) {
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Initialization failed\n");
        return -1;
    }

    while (1) {
        read_mma8x5x();
        xtimer_sleep(5);
    }

    return 0;
}