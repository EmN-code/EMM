#include "periph/i2c.h"
#include "xtimer.h"
#include "mma8x5x.h"

#define READ_INTERVAL (5U * US_PER_SEC)  // 5 seconds

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };
    
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        // Handle initialization failure
        return 1;
    }
    
    while (1) {
        mma8x5x_read(&dev, &data);
        
        // Process data (e.g., print values)
        printf("X: %d, Y: %d, Z: %d\n", data.x_baty,data.y,data.z);
        
        xtimer_sleep(READ_INTERVAL);
    }
    return 0;
}