#include "thread.h"
#include "periph/i2c.h"
#include "core_def.h"

static mma8x5x_t dev;
static mma8x5x_data_t data;
static const mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

#define SLEEP_TIME_MS   (5000U)  // 5 seconds

int main(void) {
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Error initializing MMA8x5x\n");
        return 1;
    }

    while (1) {
        if(mma8x5x_read(&dev, &data) == MMA8X5X_OK){
            // Assuming data structure has x, y, z to store accelerometer readings
            printf("x: %d\ty: %d\tz: %d\n", data.x, data.y, data.z);
        } else {
            printf("Error reading data\n");
        }
        xtimer_sleep(SLEEP_TIME_MS / 1000); // Sleep for 5 seconds
    }

    return 0;
}