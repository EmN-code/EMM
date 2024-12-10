#include "periph/i2c.h"
#include "mma8x5x.h"
#include "xtimer.h"
#include "stdio.h"

#define I2C_DEV     I2C_DEV(0)  // Use I2C device 0
#define I2C_SPEED   I2C_SPEED_FAST  // Set I2C speed to fast mode (400 kHz)
#define MMA8X5X_I2C_ADDR 0x1D  // Default I2C address for MMA8x5x

void i2c_init(void) {
    i2c_init_dev(I2C_DEV);
    i2c_acquire(I2C_DEV);
    i2c_speed(I2C_DEV, I2C_SPEED);
}

void mma8x5x_init(void) {
    mma8x5x_init_t init_params = {
        .dev = I2C_DEV,
        .addr = MMA8X5X_I2C_ADDR
    };

    if (mma8x5x_init(&init_params) != MMA8X5X_OK) {
        puts("Failed to initialize MMA8x5x");
        return;
    }

    // Additional configuration if needed
    mma8x5x_set_odr(MMA8X5X_ODR_50);  // Set output data rate to 50 Hz
    mma8x5x_set_range(MMA8X5X_RANGE_2G);  // Set range to ±2g
}

void read_mma8x5x_data(void) {
    mma8x5x_data_t data;

    if (mma8x5x_read_data(&data) != MMA8X5X_OK) {
        puts("Failed to read data from MMA8x5x");
        return;
    }

    printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
}

int main(void) {
    i2c_init();
    mma8x5x_init();

    while (1) {
        read_mma8x5x_data();
        xtimer_sleep(5);  // Sleep for 5 seconds
    }

    return 0;
}