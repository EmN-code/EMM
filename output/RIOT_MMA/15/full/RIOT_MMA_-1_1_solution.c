#include <thread.h>
#include <periph_conf.h>
#include "mma8x5x.h"

#define SLEEP_INTERVAL (5U)

static mma8x5x_t dev;
static const mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),  // Assuming I2C 0
    .addr = MMA8X5X_ADDR_DEFAULT
};

void *mma8x5x_thread(void *arg) {
    (void)arg;

    while (1) {
        if (mma8x5x_is_ready(&dev)) {
            // Assuming retrieving and printing acceleration
            int16_t x, y, z;
            mma8x5x_read_accel(&dev, &x, &y, &z);
            printf("Acceleration: x=%d, y=%d, z=%d\n", x, y, z);
        }
        else {
            printf("Device not ready.\n");
        }
        xtimer_sleep(SLEEP_INTERVAL);
    }
    return NULL;
}

int main(void) {
    printf("Initializing MMA8x5x sensor...\n");

    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }

    kernel_pid_t pid = thread_create(stack,
                                     sizeof(stack),
                                     THREAD_PRIORITY_MAIN - 1,
                                     THREAD_CREATE_STACKTEST,
                                     mma8x5x_thread,
                                     NULL,
                                     "mma8x5x_thread");

    if (pid < 0) {
        printf("Failed to create thread\n");
        return 1;
    }

    return 0;
}