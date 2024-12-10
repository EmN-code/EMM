#include xtimer.h
#include evtimer.h
#include periph/i2c.h

#include <stdio.h>

#define INTERVAL (5U * US_PER_SEC) // 5 seconds interval

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    // Initialize the I2C interface for MMA8x5x
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = 0x1D, // Assuming this is the default I2C address for MMA8x5x
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };
    
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("MMA8x5x initialization failed!\n");
        return 1;
    }
    
    while (1) {
        xtimer_usleep(INTERVAL);
        
        if (mma8x5x_read(&dev, &data) != MMA8X5X_OK) {
            printf("Failed to read from MMA8x5x\n");
        } else {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        }
    }
    
    return 0;
}
