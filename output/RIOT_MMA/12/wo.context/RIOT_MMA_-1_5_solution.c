#include "xtimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"

#define SLEEP_INTERVAL (5U)  // 5 seconds

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void)
{
    printf("MMA8x5x accelerometer test on ESP32\n");

    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    // Initialize the accelerometer
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Initialization failed\n");
        return 1;
    }

    while (1) {
        // Read accelerometer data
        if (mma8x5x_read(&dev, &data) != MMA8X5X_OK) {
            printf("Error reading data\n");
        } else {
            printf("X: %d\tY: %d\tZ: %d\n", data.x, data.y, data.z);
        }

        // Sleep for the specified interval
        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}