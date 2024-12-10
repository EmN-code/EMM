#include "mma8x5x.h"
#include "ztimer.h"
#include "periph/i2c.h"
#include <stdio.h>

#define I2C_DEV         I2C_DEV(0)  // Adjust based on your setup
#define MMA8X5X_ADDR    0x1D        // Default I2C address for MMA8x5x
#define TIMER_INTERVAL  (5U * 1000U * 1000U) // 5 seconds in microseconds

static mma8x5x_t dev;
static mma8x5x_data_t data;

void read_sensor(void)
{
    if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
        printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
    } else {
        printf("Failed to read data from MMA8x5x\n");
    }
}

int main(void)
{
    // Initialize I2C
    i2c_init(I2C_DEV);

    // Configure the MMA8x5x device
    mma8x5x_params_t params = {
        .i2c = I2C_DEV,
        .addr = MMA8X5X_ADDR,
        .rate = 0,  // Set appropriate rate
        .range = 0  // Set appropriate range
    };
    dev.params = params;

    // Initialize the MMA8x5x sensor
    if (mma8x5x_init(&dev) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }

    // Set up a periodic timer to read the sensor every 5 seconds
    ztimer_t timer;
    ztimer_set(ZTIMER_MSEC, &timer, TIMER_INTERVAL, read_sensor, NULL);

    // Main loop
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, TIMER_INTERVAL);
    }

    return 0;
}