#include <stdio.h>
#include "ztimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"

// Define I2C parameters
#define I2C_DEV         I2C_DEV(0)
#define I2C_ADDR        MMA8X5X_ADDR

// Define the timer interval (5 seconds)
#define TIMER_INTERVAL  (5U * US_PER_SEC)

// MMA8x5x device descriptor
static mma8x5x_t dev;

// Timer
static ztimer_t timer;

// Timer callback function
static void timer_callback(void *arg)
{
    (void)arg;
    mma8x5x_data_t data;

    // Read sensor data
    if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
        printf("Acceleration [g]: X=%d, Y=%d, Z=%d\n", data.x, data.y, data.z);
    } else {
        printf("Failed to read sensor data\n");
    }

    // Restart the timer
    ztimer_set(ZTIMER_SEC, &timer, TIMER_INTERVAL);
}

int main(void)
{
    // Initialize I2C
    if (i2c_init(I2C_DEV) != 0) {
        printf("Failed to initialize I2C\n");
        return 1;
    }

    // Initialize MMA8x5x sensor
    mma8x5x_params_t params = {
        .i2c = I2C_DEV,
        .addr = I2C_ADDR,
        .range = MMA8X5X_RANGE_2G,
        .rate = MMA8X5X_RATE_100HZ,
        .offset = {0, 0, 0}
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x sensor\n");
        return 1;
    }

    // Initialize and start the timer
    ztimer_set(ZTIMER_SEC, &timer, TIMER_INTERVAL);
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Start the timer
    ztimer_set(ZTIMER_SEC, &timer, TIMER_INTERVAL);

    // Main loop
    while (1) {
        // Put the MCU to sleep to save power
        ztimer_sleep(ZTIMER_SEC, TIMER_INTERVAL);
    }

    return 0;
}