#include <ztimer.h>
#include <thread.h>
#include <stdio.h>
#include <mma8x5x.h>
#include <kernel.h>
#include <xtimer.h>

#define STACKSIZE (THREAD_STACKSIZE_MAIN + THREAD_EXTRA_STACKSIZE_PRINTF)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACKSIZE];
static mma8x5x_t dev;

static void *mma8x5x_read_loop(void *arg) {
    (void)arg;
    mma8x5x_data_t data;

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return NULL;
}

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Failed to initialize accelerometer");
        return 1;
    }

    thread_create(stack, sizeof(stack), PRIORITY, THREAD_CREATE_STACKTEST, mma8x5x_read_loop, NULL, "mma8x5x reader");
    
    return 0;
}