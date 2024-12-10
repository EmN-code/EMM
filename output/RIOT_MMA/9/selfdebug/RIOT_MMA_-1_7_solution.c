#include <xtimer.h>
#include <mma8x5x.h>
#include <stdio.h>

#define SLEEP_TIME (5U * 1000000U) // 5 seconds in microseconds

static mma8x5x_t dev;
static mma8x5x_data_t data;
static const mma8x5x_params_t params = {
    .i2c = I2C_DEV(0), // assuming I2C_DEV(0) is valid on the ESP32 platform
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void) {
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }
    
    while (1) {
        if (mma8x5x_read(&dev, &data) != MMA8X5X_OK) {
            printf("Failed to read data\n");
        } else {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        }
        
        xtimer_usleep(SLEEP_TIME);
    }
    
    return 0;
}