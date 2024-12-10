#include "thread.h"
#include "timex.h"
#include "xtimer.h"
#include "mma8x5x.h"
#include "stdio.h"

#define INTERVAL (5U * US_PER_SEC)  // 5 seconds

static mma8x5x_t dev;

void read_mma8x5x(void) {
    int16_t x, y, z;
    if (mma8x5x_read(&dev, &x, &y, &z) == 0) {
        printf("X: %d, Y: %d, Z: %d\n", x, y, z);
    } else {
        printf("Error reading MMA8x5x\n");
    }
}

void *mma8x5x_thread(void *arg) {
    (void)arg;
    while (1) {
        read_mma8x5x();
        xtimer_sleep(INTERVAL);
    }
    return NULL;
}

int main(void) {
    mma8x5x_params_t params = MMA8X5X_PARAMS;
    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Initialization failed\n");
        return 1;
    }

    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  mma8x5x_thread, NULL, "mma8x5x_thread");

    return 0;
}