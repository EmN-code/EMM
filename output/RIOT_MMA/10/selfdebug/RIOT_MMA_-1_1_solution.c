#include "thread.h"
#include "mma8x5x.h"
#include "periph_conf.h"

#include <stdio.h>

static mma8x5x_t dev;

void *read_sensor(void *arg) {
    (void)arg;
    while (1) {
        if (mma8x5x_is_ready(&dev)) {
            int x = mma8x5x_read_axis(&dev, 0);  // Read X-axis
            int y = mma8x5x_read_axis(&dev, 1);  // Read Y-axis
            int z = mma8x5x_read_axis(&dev, 2);  // Read Z-axis
            printf("X: %d, Y: %d, Z: %d\n", x, y, z);
        }
        thread_sleep(5);  // Sleep for 5 seconds
    }
    return NULL;
}

int main(void) {
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, read_sensor, NULL,
                  "sensor_thread");
    return 0;
}