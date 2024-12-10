#include <thread_config.h>
#include <stdio.h>
#include <stdint.h>
#include <mma8x5x.h>
#include <xtimer.h>

#define I2C_DEV_NUM    0  // Assuming using I2C_DEV(0)
#define ADDR           MMA8X5X_I2C_ADDRESS  

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    // Initialization parameters for the sensor
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(I2C_DEV_NUM),
        .addr = ADDR,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Error initializing MMA8x5x\n");
        return -1;
    }
    
    while (1) {
        // Perform a read from the sensor
        if (mma8x5x_read(&dev, &data) != MMA8X5X_OK) {
            printf("Error reading data from MMA8x5x\n");
        } else {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        }
        xtimer_sleep(5);  // Wait for 5 seconds
    }

    return 0;
}