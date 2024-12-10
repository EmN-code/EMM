#include <stdio.h>
#include <stdlib.h>
#include "periph/timer.h"
#include "mma8x5x.h"

static mma8x5x_t dev;
static mma8x5x_data_t data;

void read_sensor(void) {
    mma8x5x_read(&dev, &data);
    printf("X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
}

void timer_callback(void *arg) {
    (void)arg; // Avoid unused parameter warning
    read_sensor();
}

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization failed!");
        return 1;
    }

    tim_t timer = TIMER_DEV(0);
    uint32_t freq = 1E6; // Use 1 MHz virtual frequency
    timer_init(timer, freq, timer_callback, NULL);
    timer_set(timer, (1E6 * 5)); // Set timer to 5 seconds
    
    while (1) {
        // Main loop can handle other tasks
    }

    return 0;
}