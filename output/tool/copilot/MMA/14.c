#include <stdio.h>
#include "ztimer.h"
#include "mma8x5x.h"
#include "periph/i2c.h"

#define I2C_DEV         I2C_DEV(0)
#define I2C_ADDR        MMA8X5X_DEFAULT_ADDR

static mma8x5x_t dev;
static ztimer_t timer;

static void read_sensor(void)
{
    mma8x5x_data_t data;
    if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
        printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
    } else {
        printf("Failed to read sensor data\n");
    }
}

static void timer_callback(void *arg)
{
    (void)arg;
    read_sensor();
    ztimer_set(ZTIMER_MSEC, &timer, 5000); // Set timer for next 5 seconds
}

int main(void)
{
    printf("Initializing MMA8x5x sensor\n");

    mma8x5x_params_t params = {
        .i2c = I2C_DEV,
        .addr = I2C_ADDR,
        .range = MMA8X5X_RANGE_2G,
        .rate = MMA8X5X_RATE_1HZ,
        .offset = {0, 0, 0}
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x sensor\n");
        return 1;
    }

    printf("MMA8x5x sensor initialized successfully\n");

    // Set up the timer
    timer.callback = timer_callback;
    ztimer_set(ZTIMER_MSEC, &timer, 5000); // Start timer for 5 seconds

    // Main loop
    while (1) {
        // Put the MCU to sleep until an interrupt occurs
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}