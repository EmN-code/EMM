#include <xtimer.h>
#include <stdio.h>
#include "periph/i2c.h"
#include "mma8x5x.h"

#define I2C_BUS 0
#define CONFIG_MMA8X5X_I2C_ADDRESS 0x1D  // Adjusted to the correct macro
#define TIME_INTERVAL (5U * US_PER_SEC)  // 5 seconds interval

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(I2C_BUS),
    .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void) {
    printf("Initializing MMA8x5x accelerometer...\n");
    
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Error: Failed to initialize MMA8x5x!\n");
        return 1;
    }

    mma8x5x_data_t acc_data;
    
    while (1) {
        if (mma8x5x_read(&dev, &acc_data) != MMA8X5X_OK) {
            printf("Error: Failed to read data from MMA8x5x!\n");
        } else {
            printf("Acceleration Data: X=%d, Y=%d, Z=%d\n", acc_data.x, acc_data.y, acc_data.z);
        }
        xtimer_sleep(TIME_INTERVAL);
    }
    return 0;
}