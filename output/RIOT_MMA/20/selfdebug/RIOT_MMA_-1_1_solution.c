#include "thread_arch.h"
#include "thread.h"
#include "periph_conf.h"
#include "host/ble_ibeacon.h"
#include <stdio.h>

#include "mma8x5x.h"
#include "xtimer.h"

#define INTERVAL (5U * 1000000U) // 5 seconds in microseconds

static mma8x5x_t dev;

void *mma8x5x_read_thread(void *arg) {
    (void)arg;
    while (1) {
        if (mma8x5x_is_ready(&dev)) {
            int16_t x, y, z;
            if (mma8x5x_read(&dev, &x, &y, &z) == 0) {
                printf("MMA8x5x Data - X: %d, Y: %d, Z: %d\n", x, y, z);
            }
        } else {
            printf("MMA8x5x Not Ready\n");
        }
        xtimer_sleep(INTERVAL / 1000000); // sleep for INTERVAL seconds
    }
    return NULL;
}

int main(void) {
    printf("Initializing MMA8x5x sensor...\n");

    mma8x5x_params_t params;
    // Assign parameters for device initialization

    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Sensor initialization failed\n");
        return 1;
    }

    printf("Sensor initialized, starting read thread\n");
    thread_create(/* stack size */ NULL,
                  /* priority */ THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST,
                  mma8x5x_read_thread,
                  NULL,
                  "mma8x5x_read_thread");

    /* Main thread should do other tasks or just sleep */
    while (1) {
        xtimer_sleep(60); // sleep for 60 seconds in the main thread
    }

    return 0;
}