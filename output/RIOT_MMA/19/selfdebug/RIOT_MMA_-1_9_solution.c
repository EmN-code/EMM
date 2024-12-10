#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x.h"
#include "kernel_defines.h"
#include <stdio.h>
#include <stdlib.h>

#define THREAD_PRIORITY_APP            (THREAD_PRIORITY_MAIN + 1)
#define CONFIG_MMA8X5X_I2C_ADDRESS     (0x1D) // Assume the I2C address for the MMA8653

static kernel_pid_t _sensor_thread_pid;

static void *_sensor_thread(void *arg) {
    (void)arg;

    mma8x5x_t dev;
    mma8x5x_data_t data;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x sensor\n");
        return NULL;
    }

    while (1) {
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        } else {
            printf("Error reading data\n");
        }
        xtimer_sleep(5);
    }
}

int main(void) {
    _sensor_thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_APP,
                                       THREAD_CREATE_STACKTEST, _sensor_thread,
                                       NULL, "sensor_thread");

    if (_sensor_thread_pid < 0) {
        printf("Error initializing sensor thread\n");
        return 1;
    }

    return 0;
}
