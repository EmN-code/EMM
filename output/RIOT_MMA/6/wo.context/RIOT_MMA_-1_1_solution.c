#include "thread.h"
#include "periph_conf.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "xtimer.h"

#define INTERVAL 5

static mma8x5x_t dev;

void *read_sensor(void *arg) {
    (void)arg;
    int16_t x, y, z;
    while (1) {
        if (mma8x5x_is_ready(&dev)) {
            mma8x5x_read(&dev, &x, &y, &z);
            printf("X: %d, Y: %d, Z: %d\n", x, y, z);
        } else {
            printf("MMA8x5x not ready\n");
        }
        xtimer_sleep(INTERVAL);
    }
    return NULL;
}

int main(void) {
    if (mma8x5x_init(&dev, &mma8x5x_params[0]) == 0) {
        puts("MMA8x5x initialized succesfully");
    } else {
        puts("Failed to initialize MMA8x5x");
        return 1;
    }

    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, read_sensor, NULL, "read_sensor");
    return 0;
}