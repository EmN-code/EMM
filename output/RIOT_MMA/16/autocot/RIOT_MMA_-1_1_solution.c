#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "xtimer.h"
#include "stdio.h"

#define SLEEP_INTERVAL (5U * 1000000U) // 5 seconds in microseconds

int main(void) {
    mma8x5x_t dev;
    const mma8x5x_params_t params = {
        /* Set device-specific parameters here if needed */
    };

    // Initialize the sensor
    if (mma8x5x_init(&dev, &params) != 0) {
        puts("Failed to initialize MMA8x5x sensor");
        return 1;
    }

    puts("MMA8x5x sensor initialized");

    while (1) {
        // Read from the MMA8x5x sensor
        if (mma8x5x_is_ready(&dev)) {
            int16_t x, y, z;
            mma8x5x_read(&dev, &x, &y, &z);
            printf("Acceleration: x=%d, y=%d, z=%d\n", x, y, z);
        } else {
            puts("MMA8x5x sensor not ready");
        }

        // Sleep for 5 seconds
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}