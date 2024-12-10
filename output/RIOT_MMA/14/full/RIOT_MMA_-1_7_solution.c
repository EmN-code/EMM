#include "ztimer.h"
#include "xtimer.h"
#include "stdio.h"
#include "stdlib.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "stdbool.h"
#include "stddef.h"
#include "string.h"
#include "thread.h"
#include "esp_system.h"

#define THREAD_STACKSIZE_MAIN (2 * THREAD_STACKSIZE_DEFAULT)
#define DEFAULT_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define SLEEP_INTERVAL (5U * US_PER_SEC) // 5 seconds interval

static char stack[THREAD_STACKSIZE_MAIN];

static void *read_accel_thread(void *arg) {
    (void) arg;
    mma8x5x_t dev;
    mma8x5x_data_t data;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS, // Ensure this constant is defined in your configuration
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize the accelerometer\n");
        return NULL;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration: X= %d mg, Y= %d mg, Z= %d mg\n", data.x, data.y, data.z);
        ztimer_sleep(ZTIMER_MSEC, 5000); // Sleep for 5 seconds
    }

    return NULL;
}

int main(void) {
    printf("Starting accelerometer readings\n");
    thread_create(stack, sizeof(stack), DEFAULT_PRIORITY, THREAD_CREATE_STACKTEST, read_accel_thread, NULL, "accel_thread");
    return 0;
}