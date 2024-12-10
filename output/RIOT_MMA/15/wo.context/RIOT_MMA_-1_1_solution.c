#include <thread.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "periph_conf.h"
#include "xtimer.h"
#include "mma8x5x.h"

#define INTERVAL (5U * US_PER_SEC)

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    // Set parameters based on your configuration
};

void read_mma8x5x(void) {
    int16_t x, y, z;
    while (1) {
        if (mma8x5x_read(&dev, &x, &y, &z) != 0) {
            printf("Failed to read from MMA8x5x\n");
        } else {
            printf("X: %" PRId16 " Y: %" PRId16 " Z: %" PRId16 "\n", x, y, z);
        }
        xtimer_sleep(INTERVAL);
    }
}

int main(void) {
    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }

    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, 0, read_mma8x5x, NULL, "mma_reader");

    return 0;
}