#include "xtimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"
#include "kernel_defines.h"

#define SLEEPTIME_S (5U)

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    int res;
    
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if ((res = mma8x5x_init(&dev, &params)) != MMA8X5X_OK) {
        puts("Initialization failed");
        return 1;
    }

    while (1) {
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        }
        else {
            puts("Read error");
        }
        
        xtimer_sleep(SLEEPTIME_S);
    }
    return 0;
}