#include <xtimer.h>
#include <periph/i2c.h>
#include <stdio.h>

#include "mma8x5x.h"

#define SLEEP_INTERVAL   (5U)

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    printf("Initializing MMA8x5x sensor...\n");
    
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS, // Using the CONFIG macro here
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Initialization failed!\n");
        return 1;
    }

    while (1) {
        /* Insert appropriate data reading commands here when using the RIOT MMA8x5x library */
        if(mma8x5x_read(&dev, &data) == MMA8X5X_OK){
            printf("Accel data: X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        } else {
            printf("Failed to read data from sensor!\n");
        }
        
        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}