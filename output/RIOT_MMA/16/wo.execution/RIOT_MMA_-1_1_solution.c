#include <thread.h>
#include <stdio.h>
#include <periph_conf.h>
#include <inttypes.h>

#include "mma8x5x.h"

#define SLEEP_INTERVAL (5U)

static mma8x5x_t dev;

void *read_sensor(void *arg) {
    (void)arg; // Unused parameter
    int16_t x, y, z;
    while (1) {
        if (mma8x5x_is_ready(&dev)) {
            mma8x5x_read(&dev, &x, &y, &z);
            printf("MMA8x5x - X: %d, Y: %d, Z: %d\n", x, y, z);
        } else {
            printf("MMA8x5x not ready\n");
        }
        xtimer_sleep(SLEEP_INTERVAL);
    }
    return NULL;
}

int main(void) {
    // Assuming MMA8x5x is initialized elsewhere in the code
    // or you can initialize here if specific initialization params are known

    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, read_sensor, NULL, "read_sensor");

    // main thread can wait forever
    thread_sleep();
    return 0;
}