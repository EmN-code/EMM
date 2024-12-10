#include "thread.h"
#include "periph/timer.h"
#include "mma8x5x.h"
#include "periph_conf.h"
#include "xtimer.h"

// Configuration for the MMA8x5x sensor
static const mma8x5x_params_t mma8x5x_params = {
    /* Assume default params for the sensor */
};

static mma8x5x_t dev;

void *read_mma8x5x(void *arg) {
    (void)arg;
    while (1) {
        int ready;
        ready = mma8x5x_is_ready(&dev);
        if (ready == 0) {
            printf("MMA8x5x: not ready\n");
        } else {
            int16_t acc[3];
            if (mma8x5x_read(&dev, acc) == 0) {
                printf("Acceleration [mg]: X=%d, Y=%d, Z=%d\n", acc[0], acc[1], acc[2]);
            } else {
                printf("Error reading MMA8x5x\n");
            }
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    puts("Starting MMA8x5x sensor reading application\n");

    if (mma8x5x_init(&dev, &mma8x5x_params) != 0) {
        puts("MMA8x5x initialization failed\n");
        return 1;
    }

    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, read_mma8x5x, NULL, "mma8x5x_thread");

    return 0;
}