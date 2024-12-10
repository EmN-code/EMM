#include "mma8x5x.h"
#include "kernel.h"
#include "periph/i2c.h"
#include <stdio.h>

static mma8x5x_t dev;
static mma8x5x_data_t data;

void read_sensor(void)
{
    if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
        printf("Accl X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
    } else {
        printf("Error reading sensor\n");
    }
}

int main(void)
{
    printf("Initializing I2C and MMA8x5x sensor\n");
    
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS, // Fixed macro
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Error initializing MMA8x5x sensor\n");
        return 1;
    }

    // 5-second interval timer
    ztimer_sleep(ztimer_now() + 5 * 1000); // 5 seconds

    while (1) {
        read_sensor();
        ztimer_sleep(ztimer_now() + 5 * 1000); // 5 seconds delay
    }

    return 0;
}