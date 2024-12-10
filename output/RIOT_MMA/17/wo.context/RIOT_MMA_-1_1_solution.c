#include "thread.h"
#include "periph/i2c.h"
#include "mma8x5x.h"
#include "msg.h"
#include <stdio.h>

#define I2C_DEVICE        I2C_DEV(0)
#define I2C_ADDRESS       0x1D  // Typical I2C address for MMA8x5X
#define SLEEP_MS          5000

static char stack[THREAD_STACKSIZE_MAIN];

static void *mma8x5x_thread(void *arg) {
    (void) arg;
    mma8x5x_t dev;
    mma8x5x_params_t params = {
        .i2c  = I2C_DEVICE,
        .addr = I2C_ADDRESS
    };

    if (mma8x5x_init(&dev, &params) != 0) {
        puts("Failed to initialize MMA8x5x sensor");
        return NULL;
    }

    while (1) {
        if (mma8x5x_is_ready(&dev)) {
            int16_t x, y, z;
            mma8x5x_read(&dev, &x, &y, &z);
            printf("Acceleration Data - X: %d, Y: %d, Z: %d\n", x, y, z);
        }
        xtimer_sleep(SLEEP_MS / 1000);  // Convert ms to seconds
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mma8x5x_thread, NULL, "mma8x5x");
    return 0;
}