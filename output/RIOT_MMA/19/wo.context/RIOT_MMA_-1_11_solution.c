#include "xtimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"

#define DELAY 5 // seconds
#define I2C_ADDR MMA8X5X_I2C_ADDRESS

static mma8x5x_t dev;
static mma8x5x_data_t data;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = I2C_ADDR,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void) {
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        // Handle error
        return 1;
    }
    
    while (1) {
        if (mma8x5x_read(&dev, &data) != MMA8X5X_OK) {
            // Handle read error
            continue;
        }

        // Process data
        printf("Acceleration X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);

        xtimer_sleep(DELAY);
    }
    return 0;
}