#include "thread.h"
#include "periph_conf.h"
#include "mma8x5x.h"

#define SLEEP_INTERVAL (5 * US_PER_SEC)

kernel_pid_t mma8x5x_thread_pid;
static char mma8x5x_thread_stack[THREAD_STACKSIZE_MAIN];

void *mma8x5x_thread(void *arg) {
    mma8x5x_t *dev = (mma8x5x_t *)arg;

    while (1) {
        if (mma8x5x_is_ready(dev)) {
            int16_t x, y, z;
            mma8x5x_read(dev, &x, &y, &z);
            printf("Acceleration Data - X: %d, Y: %d, Z: %d\n", x, y, z);
        } else {
            printf("MMA8x5x sensor is not ready\n");
        }
        xtimer_sleep(SLEEP_INTERVAL / US_PER_MS);
    }

    return NULL;
}

void init_mma8x5x(void) {
    mma8x5x_params_t params;
    params.i2c = I2C_DEV;
    params.addr = MMA8X5X_ADDR;
    
    mma8x5x_t dev;
    if (mma8x5x_init(&dev, &params) < 0) {
        printf("Initialization of MMA8x5x failed\n");
        return;
    }

    mma8x5x_thread_pid = thread_create(mma8x5x_thread_stack, THREAD_STACKSIZE_MAIN,
                                       THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                       mma8x5x_thread, &dev, "mma8x5x_thread");
}

int main(void) {
    puts("MMA8x5x Sensor Reader starting...");
    init_mma8x5x();
    
    return 0;
}