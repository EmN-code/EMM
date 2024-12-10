#include "thread.h"
#include "timex.h"
#include "ztimer.h"
#include "mma8x5x.h"
#include "stdio.h"

#define DELAY_SEC        (5U)  // Delay of 5 seconds

// Sensor parameters
static const mma8x5x_params_t mma8x5x_params = {
    /** Example parameters - these might need to be adjusted for actual setup **/
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .range = MMA8X5X_RANGE_2G,
    .scale = MMA8X5X_SCALE_8,
};

static mma8x5x_t dev;

// Thread stack and priority
static char stack[THREAD_STACKSIZE_MAIN];

static void *sensor_read_thread(void *arg) {
    (void)arg;
    int16_t x, y, z;

    while (1) {
        // Read accelerometer values
        if (mma8x5x_read(&dev, &x, &y, &z) == 0) {
            printf("Acceleration: X=%d, Y=%d, Z=%d\n", x, y, z);
        }
        else {
            printf("Failed to read from sensor\n");
        }
        ztimer_sleep(ZTIMER_MSEC, DELAY_SEC * MS_PER_SEC);
    }
    return NULL;
}

int main(void) {
    puts("RIOT MMA8x5x Sensor Example\n");

    // Initialize the MMA8x5x sensor
    if (mma8x5x_init(&dev, &mma8x5x_params) != 0) {
        puts("Failed to initialize the MMA8x5x sensor\n");
        return -1;
    }

    // Create the sensor reading thread
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, sensor_read_thread, NULL, "sensor_read");

    // Main thread does nothing for this minimal example
    while (1) {
        // Idle now
    }

    return 0;
}