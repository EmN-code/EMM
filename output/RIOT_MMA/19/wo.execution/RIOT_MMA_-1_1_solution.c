#include "thread.h"
#include "mma8x5x.h"
#include "xtimer.h"
#include <stdio.h>

#define SLEEP_DURATION (5U * US_PER_SEC)  // 5 seconds

mma8x5x_t dev;
mma8x5x_params_t params = {
    // assume params are defined here
};

static void *_read_mma8x5x(void *arg) {
    (void)arg;
    while (1) {
        if (mma8x5x_is_ready(&dev)) {
            // Reading sensor data; this assumes the function to read is available
            // You would replace the below with actual reading, e.g., mma8x5x_read(&dev)
            printf("Reading MMA8x5x sensor data\n");
        } else {
            printf("Sensor not ready\n");
        }
        xtimer_sleep(SLEEP_DURATION);
    }
    return NULL;
}

int main(void) {
    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, _read_mma8x5x, NULL,
                  "mma8x5x");

    return 0;
}