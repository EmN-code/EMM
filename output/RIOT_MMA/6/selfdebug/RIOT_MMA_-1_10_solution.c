#include "xtimer.h"
#include "stdio.h"
#include "periph/i2c.h"
#include "mma8x5x.h"

#define I2C_ADDR (0x1D) // Default I2C address for MMA8653
#define I2C_BUS I2C_DEV(0)
#define SLEEP_DURATION (5 * US_PER_SEC)

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    printf("Initializing MMA8x5x sensor...\n");

    // Initialize I2C interface
    mma8x5x_params_t params = {
        .i2c = I2C_BUS,
        .addr = I2C_ADDR,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x sensor\n");
        return -1;
    }

    while (1) {
        /* Read data every 5 seconds */
        xtimer_sleep(SLEEP_DURATION / US_PER_SEC);

        if (mma8x5x_read(&dev, &data) != 0) {
            printf("Error reading data from MMA8x5x\n");
        }
        else {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        }
    }

    return 0;
}