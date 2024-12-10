#include <ztimer.h>
#include <xtimer.h>
#include <stdio.h>
#include <stdbool.h>  
#include <stddef.h>
#include <stdatomic.h>
#include <stdlib.h>  
#include <stdint.h> 
#include <string.h> 
#include "mma8x5x.h"
#include "mma8x5x_params.h"

#define READ_INTERVAL (5U * US_PER_SEC)

static mma8x5x_t dev;
static const mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

static void read_mma8x5x(void)
{
    mma8x5x_data_t data;
    mma8x5x_read(&dev, &data);

    printf("Acceleration [mg]: x=%d y=%d z=%d\n", data.x, data.y, data.z);
}

int main(void)
{
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Initialization failed\n");
        return -1;
    }

    printf("Initialization successful\n");

    while (1) {
        read_mma8x5x();
        xtimer_sleep(5);  // Sleep for 5 seconds
    }

    return 0;
}