#include "mma8x5x.h"
#include "xtimer.h"
#include "stdio.h"
#include "periph/i2c.h"

#define SLEEP_INTERVAL 5 // Sleep for 5 seconds

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void) {
    int res;
    
    // Initialize the MMA8x5x device
    res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Error initializing MMA8x5x: %d\n", res);
        return 1;
    }
    
    printf("MMA8x5x initialized successfully.\n");
    
    while (1) {
        // Read accelerometer data
        mma8x5x_data_t data;
        res = mma8x5x_read(&dev, &data);
        if (res != MMA8X5X_OK) {
            printf("Error reading from MMA8x5x: %d\n", res);
        }
        else {
            printf("Acceleration data: X=%d, Y=%d, Z=%d\n", data.x, data.y, data.z);
        }
        
        // Sleep for the specified interval
        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}