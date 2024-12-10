#include "xtimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"

#define I2C_DEV         (I2C_DEV(0))
#define ADDR_MMA8X5X    (0x1D)

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    int res;
    
    mma8x5x_params_t params = {
        .i2c = I2C_DEV,
        .addr = ADDR_MMA8X5X,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };
    
    res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Failed to initialize the MMA8X5x device (Error: %d)\n", res);
        return 1;
    }
    
    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration data: X=%d mg, Y=%d mg, Z=%d mg\n", data.x_axis, data.y_axis, data.z_axis);
        xtimer_sleep(5);
    }

    return 0;
}