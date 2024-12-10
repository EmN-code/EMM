#include <stdio.h>
#include "ztimer.h"
#include "mma8x5x.h"
#include "periph/i2c.h"

// Define the I2C parameters
#define I2C_DEV         I2C_DEV(0)
#define I2C_ADDR        MMA8X5X_DEFAULT_ADDR

// Define the timer interval (5 seconds)
#define TIMER_INTERVAL  (5U * US_PER_SEC)

// MMA8x5x device descriptor
static mma8x5x_t dev;

// Timer descriptor
static ztimer_t timer;

// Timer callback function
static void timer_callback(void *arg)
{
    (void)arg;
    mma8x5x_data_t data;

    // Read data from the sensor
    if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
        printf("Acceleration [mg]: X=%d, Y=%d, Z=%d\n", data.x, data.y, data.z);
    } else {
        printf("Failed to read data from MMA8x5x sensor\n");
    }

    // Restart the timer
    ztimer_set(ZTIMER_USEC, &timer, TIMER_INTERVAL);
}

int main(void)
{
    // Initialize the I2C bus
    i2c_init(I2C_DEV);

    // Initialize the MMA8x5x sensor
    mma8x5x_params_t params = {
        .i2c = I2C_DEV,
        .addr = I2C_ADDR,
        .range = MMA8X5X_RANGE_2G,
        .rate = MMA8X5X_RATE_1HZ,
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x sensor\n");
        return 1;
    }

    // Initialize the timer
    ztimer_set(ZTIMER_USEC, &timer, TIMER_INTERVAL);
    timer.callback = timer_callback;

    // Start the timer
    ztimer_set(ZTIMER_USEC, &timer, TIMER_INTERVAL);

    // Main loop
    while (1) {
        // Put the MCU to sleep to save power
        ztimer_sleep(ZTIMER_USEC, TIMER_INTERVAL);
    }

    return 0;
}