#include "thread.h"
#include "periph_conf.h"
#include "periph/i2c.h"
#include "xtimer.h"
#include "mma8x5x.h"
#include <stdio.h>

#define I2C_DEV        I2C_DEV(0)
#define INTERVAL       (5U * US_PER_SEC)

static mma8x5x_t dev;

void *sensor_thread(void *arg) {
    (void)arg;
    while (1) {
        if (mma8x5x_is_ready(&dev)) {
            int16_t x, y, z;
            mma8x5x_read(&dev, &x, &y, &z);
            printf("X: %d, Y: %d, Z: %d\n", x, y, z);
        } else {
            printf("Sensor not ready.\n");
        }
        xtimer_sleep(INTERVAL);
    }
    return NULL;
}

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV,
        .addr = 0x1D  // This is a common address, may need adjustment based on hardware setup
    };

    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Initialization failed.\n");
        return 1;
    }

    kernel_pid_t pid = thread_create(st_stack, sizeof(st_stack), THREAD_PRIORITY_MAIN - 1,
                                     THREAD_CREATE_STACKTEST, sensor_thread, NULL, "sensor_thread");
    if (pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create sensor thread.\n");
        return 1;
    }

    return 0;
}