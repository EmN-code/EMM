#include "mma8x5x.h"
#include "ztimer.h"
#include "periph/i2c.h"
#include <stdio.h>

#define I2C_DEV         I2C_DEV(0)  // Adjust based on your board configuration
#define MMA8X5X_ADDR    0x1D        // Default I2C address for MMA8x5x
#define READ_INTERVAL   (5U * 1000U * 1000U) // 5 seconds in microseconds

static mma8x5x_t dev;
static mma8x5x_data_t data;

void read_sensor(void)
{
    mma8x5x_read(&dev, &data);
    printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
}

int main(void)
{
    // Initialize I2C
    i2c_init(I2C_DEV);

    // Initialize MMA8x5x
    mma8x5x_params_t params = {
        .i2c = I2C_DEV,
        .addr = MMA8X5X_ADDR,
        .rate = 0,  // Set appropriate rate
        .range = 0, // Set appropriate range
    };
    dev.params = params;

    // Main loop
    while (1) {
        read_sensor();
        ztimer_sleep(ZTIMER_USEC, READ_INTERVAL);
    }

    return 0;
}