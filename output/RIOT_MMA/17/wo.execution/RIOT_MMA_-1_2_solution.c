#include "thread_arch.h"
#include "thread.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "architecture.h"
#include "event/thread.h"
#include "xtimer.h"

#define SLEEP_INTERVAL 5 // Read every 5 seconds

static mma8x5x_t dev;
mma8x5x_data_t data;
mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void) {
    // Initialize the MMA8x5x device
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        // Initialization failed
        return 1;
    }
    
    while (1) {
        if (mma8x5x_is_ready(&dev)) {
            mma8x5x_read(&dev, &data);
            // Process the data as needed
            // For example, print the values (this is placeholder, modify as per use-case)
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        }
        
        // Sleep for the specified interval
        xtimer_sleep(SLEEP_INTERVAL);
    }
    return 0;
}