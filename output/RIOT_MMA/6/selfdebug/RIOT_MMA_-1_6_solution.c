#include "xtimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"

#define INTERVAL (5U * 1000000U)  // 5 seconds

static mma8x5x_t dev;
static mma8x5x_data_t data;

void read_mma8x5x(void) {
    // Attempt to read data from sensor
    if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
        printf("X: %d, Y: %d, Z: %d\n", data.x_axis, data.y_axis, data.z_axis);
    } else {
        printf("Failed to read data\n");
    }
}

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = 0x1D,  // Assuming this is the default I2C address for the sensor
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    // Initialize the device
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Initialization failed\n");
        return 1;
    }

    // Start periodic reading
    while (1) {
        read_mma8x5x();
        xtimer_sleep(INTERVAL / 1000000U);
    }

    return 0;
}