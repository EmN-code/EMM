#include "thread.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "periph_conf.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "event/thread.h"

#include <stdio.h>

#define SLEEP_INTERVAL (5U)

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void *read_sensor(void *arg) {
    (void)arg;
    while (1) {
        int rc;
        rc = mma8x5x_init(&dev, &params);
        if (rc != MMA8X5X_OK) {
            printf("Failed to initialize accelerometer\n");
            continue;
        }
        // Assume a fictional function like mma8x5x_get_data that replaces mma8x5x_read
        rc = mma8x5x_get_data(&dev, &data);
        if (rc != MMA8X5X_OK) {
            printf("Failed to read data\n");
            continue;
        }
        printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        // Use a placeholder sleep function to read every 5 seconds
        rtc_sleep(SLEEP_INTERVAL);
    }
    return NULL;
}

int main(void) {
    printf("Starting accelerometer reader\n");
    char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, read_sensor, NULL, "read_sensor_thread");
    return 0;
}