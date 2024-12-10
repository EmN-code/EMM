#include "thread.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x.h"
#include "xtimer.h"

#define READ_INTERVAL (5U * 1000000U)

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void) {
    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        return -1; // Initialization failed
    }
    
    mma8x5x_data_t data;

    while (1) {
        xtimer_usleep(READ_INTERVAL);
        mma8x5x_read(&dev, &data);
        // Process data: data.x_axis, data.y_axis, data.z_axis
    }

    return 0;
}
