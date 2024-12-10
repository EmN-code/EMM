
#include <stdio.h>
#include <xtimer.h>
#include "mma8x5x.h"
#include "periph/i2c.h"

#define MMA8X5X_I2C_DEV         I2C_DEV(0)
#define MMA8X5X_ADDR            MMA8X5X_I2C_ADDR_1
#define MMA8X5X_MODE            MMA8X5X_MODE_NORMAL
#define MMA8X5X_RATE            MMA8X5X_RATE_1_56HZ

mma8x5x_t dev;

void print_acceleration(void) {
    mma8x5x_data_t data;
    if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
        printf("Acceleration [mg]: X: %d Y: %d Z: %d\n", data.x, data.y, data.z);
    } else {
        puts("Failed to read acceleration data");
    }
}

int main(void) {
    puts("MMA8x5x accelerometer driver test");

    if (mma8x5x_init(&dev, MMA8X5X_I2C_DEV, MMA8X5X_ADDR, MMA8X5X_MODE, MMA8X5X_RATE) != MMA8X5X_OK) {
        puts("Initialization of MMA8x5x sensor failed");
        return 1;
    }

    while (1) {
        print_acceleration();
        xtimer_sleep(5);
    }

    return 0;
}
