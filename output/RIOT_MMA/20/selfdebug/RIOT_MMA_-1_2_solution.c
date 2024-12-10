#include "xtimer.h"
#include "thread.h"
#include "mma8x5x.h"

#define SLEEP_TIME (5U * US_PER_SEC)
#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static char thread_stack[THREAD_STACKSIZE];

static void *mma8x5x_thread(void *arg) {
    mma8x5x_t *dev = (mma8x5x_t *)arg;
    mma8x5x_data_t data;

    while (1) {
        mma8x5x_read(dev, &data);  // Corrected function call
        printf("X: %d\tY: %d\tZ: %d\n", data.x, data.y,data.z);
        xtimer_sleep(SLEEP_TIME);
    }
    return NULL;
}

int main(void) {
    mma8x5x_t dev;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };
 
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization failed!");
        return 1;
    }

    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1, 0,
                  mma8x5x_thread, &dev, "mma8x5x_thread");

    return 0;
}