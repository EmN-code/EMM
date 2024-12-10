#include <stdio.h>
#include <periph/i2c.h>
#include "mma8x5x.h"

#define I2C_OK 0  // Make sure I2C_OK is defined

// Define parameters for the MMA8x5x device
static const mma8x5x_params_t mma_params = {
    .i2c = I2C_DEV(0),
    .addr = 0x1D,  // Specify the correct I2C address of the MMA8x5x
    .rate = MMA8X5X_RATE_50HZ,  // Example rate
    .range = MMA8X5X_RANGE_2G
};

static mma8x5x_t mma_dev;

void setup_mma8x5x() {
    int res = mma8x5x_init(&mma_dev, &mma_params);
    if (res != MMA8X5X_OK) {
        printf("Error initializing MMA8x5x\n");
        return;
    }
}

int main(void) {
    printf("Initializing...");
    
    setup_mma8x5x();

    while (1) {
        mma8x5x_data_t data;
        mma8x5x_read(&mma_dev, &data);
        printf("Acceleration Data: X=%d, Y=%d, Z=%d\n", data.x, data.y, data.z);
        xtimer_sleep(5);  // Delay for 5 seconds
    }

    return 0;
}
