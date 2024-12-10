#include "periph/i2c.h"
#include "periph/gpio.h"
#include "xtimer.h"
#include "kernel_defines.h"
#include <stdio.h>

#define MMA8X5X_I2C_ADDRESS   (0x1C) // Default I2C address for MMA8x5x
#define MMA8X5X_RATE_50HZ     (0)
#define MMA8X5X_RANGE_2G      (0)

typedef struct {
    uint8_t x, y, z;
} mma8x5x_data_t;

int mma8x5x_read(const mma8x5x_t *dev, mma8x5x_data_t *data);
int mma8x5x_init(mma8x5x_t *dev, const mma8x5x_params_t *params);

static mma8x5x_t dev;
static mma8x5x_data_t data;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void mma8x5x_read_data(void) {
    if (mma8x5x_read(&dev, &data) != 0) {     
        printf("Error reading data from MMA8x5x sensor\n");
    } else {
        printf("Acceleration Data: x=%d y=%d z=%d\n", data.x, data.y, data.z);
    }
}

int main(void) {
    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Initialization of MMA8x5x sensor failed\n");
        return 1;
    }
    
    while (1) {
        mma8x5x_read_data();
        xtimer_sleep(5);  // Sleep for 5 seconds
    }

    return 0;
}
