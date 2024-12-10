#include "periph/i2c.h"
#include "xtimer.h"
#include <stdio.h>

#define SLEEP_INTERVAL (5U * US_PER_SEC) // 5 seconds

typedef struct {
    i2c_t i2c;
    uint8_t addr;
    uint8_t rate;
    uint8_t range;
    uint8_t offset[3];
} mma8x5x_params_t;

// Device descriptor
typedef struct {
    mma8x5x_params_t params;
} mma8x5x_t;

int mma8x5x_init(mma8x5x_t *dev, const mma8x5x_params_t *params);
void mma8x5x_read(mma8x5x_t *dev);

#define CONFIG_MMA8X5X_I2C_ADDRESS (0x1D) // Example configuration for MMA8x5x

int main(void) {
    mma8x5x_t dev;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
        .rate = 50, // Assuming a default rate
        .range = 2  // Assuming 2G range
    };

    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }

    while (1) {
        mma8x5x_read(&dev);
        // Implement reading and processing here
        printf("Reading MMA8x5x register values...\n");

        xtimer_sleep(SLEEP_INTERVAL);
    }
    return 0;
}