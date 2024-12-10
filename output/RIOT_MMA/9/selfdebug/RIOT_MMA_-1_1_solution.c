#include "thread_arch.h"
#include "thread.h"
#include "periph_conf.h"
#include "inttypes.h"

#include <stdio.h>
#include <xtimer.h>

#include "mma8x5x.h"

#define SLEEP_TIME (5U * 1000000U) // 5 seconds in microseconds

static mma8x5x_t dev;

void *mma8x5x_reader(void *arg) {
    (void)arg;
    while (1) {
        if (mma8x5x_is_ready(&dev)) {
            int16_t x, y, z;
            mma8x5x_read_acceleration(&dev, &x, &y, &z);
            printf("Acceleration - X: %d, Y: %d, Z: %d\n", x, y, z);
        } else {
            printf("MMA8x5x not ready\n");
        }
        xtimer_usleep(SLEEP_TIME);
    }
    return NULL;
}

int main(void) {
    mma8x5x_params_t params;
    params = MMA8X5X_PARAMS;
    mma8x5x_init(&dev, &params);

    kernel_pid_t pid = thread_create(NULL, THREAD_STACKSIZE_MAIN, 
                                     THREAD_PRIORITY_MAIN - 1, 0, 
                                     mma8x5x_reader, NULL, 
                                     "mma8x5x_reader");
    if (pid < 0) {
        printf("Error initializing thread!\n");
        return 1;
    }

    return 0;
}