#include "thread.h"
#include "periph_conf.h"
#include "mma8x5x.h"
#include "xtimer.h"

#define SLEEP_INTERVAL (5U * US_PER_SEC) // 5 seconds

static mma8x5x_t dev;

void *read_mma8x5x(void *arg) {
    (void) arg;

    while (1) {
        int16_t acc[3];
        mma8x5x_is_ready(&dev); // Check if the device is ready
        mma8x5x_read(&dev, acc);

        printf("Acceleration: X=%d, Y=%d, Z=%d\n", acc[0], acc[1], acc[2]);

        xtimer_sleep(SLEEP_INTERVAL / US_PER_SEC);
    }

    return NULL;
}

int main(void) {
    mma8x5x_params_t params = {
        .port = I2C_DEV(0),
        .addr = MMA8X5X_DEFAULT_ADDR,
    };

    if (mma8x5x_init(&dev, &params) != 0) {
        printf("MMA8x5x initialization failed\n");
        return 1;
    }

    printf("MMA8x5x initialized\n");

    thread_create(sched_stack, sizeof(sched_stack), THREAD_PRIORITY_MAIN - 1, 0,
                  read_mma8x5x, NULL, "mma8x5x thread");

    return 0;
}