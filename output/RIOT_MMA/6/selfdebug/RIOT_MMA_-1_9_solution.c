#include "xtimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"

#define INTERVAL (5U * US_PER_SEC) // 5 seconds

typedef struct {
    uint8_t i2c;
    uint8_t addr;
    uint8_t rate;
    uint8_t range;
} mma8x5x_params_t;

static mma8x5x_t dev;
mma8x5x_data_t data;

int main(void) {
    // Configure parameters
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    // Initialize the device
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization failed!");
        return 1;
    }

    while (1) {
        // Sleep for the defined interval
        xtimer_sleep(INTERVAL);

        // Read accelerometer data
        mma8x5x_read(&dev, &data);

        // Print the x, y, z values
        printf("x: %d, y: %d, z: %d\n", data.x, data.y, data.z);
    }
    return 0;
}