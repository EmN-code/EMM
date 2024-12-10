#include "periph/i2c.h"
#include "mma8x5x.h"
#include "ztimer.h"
#include "stdio.h"

#define I2C_INTERFACE 0  // Use I2C_DEV(0) as your I2C interface
#define READ_INTERVAL ZTIMER_SEC(5)  // Read interval of 5 seconds

static mma8x5x_t dev;
static mma8x5x_data_t data;

void read_mma8x5x(void) {
    int res = mma8x5x_read(&dev, &data);
    if (res == MMA8X5X_OK) {
        printf("X: %d, Y: %d, Z: %d\n", data.x_axis, data.y_axis, data.z_axis);
    } else {
        printf("Failed to read data from MMA8x5X\n");
    }
}

int main(void) {
    static const mma8x5x_params_t params = {
        .i2c = I2C_DEV(I2C_INTERFACE),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("MMA8x5x initialization failed\n");
        return 1;
    }
    
    while (1) {
        read_mma8x5x();
        ztimer_sleep(ZTIMER_MSEC, READ_INTERVAL);
    }

    return 0;
}