#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "periph/i2c.h"
#include "xtimer.h"

#define INTERVAL (5U * US_PER_SEC) // 5 seconds interval

int main(void) {
    // Device and data structure
    mma8x5x_t dev;
    mma8x5x_data_t data;
    
    // Parameters to initialize the accelerometer
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    // Initialize the accelerometer device
    int res;
    while ((res = mma8x5x_init(&dev, &params)) != MMA8X5X_OK) {
        printf("Initialization failed: %d\n", res);
        xtimer_sleep(1);
    }
    printf("Accelerometer initialized successfully\n");
    
    // Main loop: periodically read accelerometer data
    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Accel X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
        xtimer_sleep(INTERVAL);
    }
    
    return 0;
}