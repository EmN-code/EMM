#include <stdio.h>
#include "xtimer.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"

#define INTERVAL (5U * US_PER_SEC) // 5 seconds

static mma8x5x_t dev;

int main(void)
{
    // Initialize the MMA8x5x sensor
    if (mma8x5x_init(&dev, &mma8x5x_params[0]) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x sensor\n");
        return 1;
    }

    // Set the sensor to active mode
    mma8x5x_set_active(&dev);

    while (1) {
        // Read sensor data
        mma8x5x_data_t data;
        mma8x5x_read(&dev, &data);

        // Print sensor data
        printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);

        // Sleep for 5 seconds
        xtimer_sleep(INTERVAL);
    }

    return 0;
}