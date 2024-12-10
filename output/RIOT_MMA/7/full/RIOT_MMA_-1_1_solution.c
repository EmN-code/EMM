#include <xtimer.h>
#include <stdio.h>
#include "mma8x5x.h"
#include "periph_conf.h"

#define DELAY (5U * 1000000U) // 5 seconds delay

int main(void) {
    mma8x5x_t dev;
    mma8x5x_params_t params;

    // Initialize the parameters (this may need adjustment based on actual setups)
    params.spi = SPI_DEV(0);
    params.cs = GPIO_PIN(0, 5);
    params.g_range = MMA8X5X_G_2;

    // Initialize the device
    while (mma8x5x_init(&dev, &params) != 0) {
        printf("Failed to initialize MMA8x5x sensor\n");
        xtimer_sleep(1); // Retry after 1 second
    }

    printf("MMA8x5x sensor initialized successfully\n");

    while (1) {
        if (mma8x5x_is_ready(&dev)) {
            int16_t x, y, z;

            // Read data from the sensor
            if (mma8x5x_read(&dev, &x, &y, &z) == 0) {
                printf("X: %d, Y: %d, Z: %d\n", x, y, z);
            } else {
                printf("Error reading data from MMA8x5x sensor\n");
            }
        } else {
            printf("MMA8x5x sensor is not ready\n");
        }

        xtimer_usleep(DELAY); // Sleep for set delay
    }

    return 0;
}