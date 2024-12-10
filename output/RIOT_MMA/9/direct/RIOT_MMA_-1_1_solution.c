#include <xtimer.h>
#include <stdio.h>
#include <stdint.h>
#include "mma8x5x.h"
#include "mma8x5x_params.h"

#define INTERVAL (5U * US_PER_SEC)

static mma8x5x_t dev;

void read_acceleration(void) {
    int16_t acc[3];
    if (mma8x5x_read(&dev, acc) != 0) {
        printf("Error reading data\n");
        return;
    }
    printf("Acceleration: X: %d mg, Y: %d mg, Z: %d mg\n", acc[0], acc[1], acc[2]);
}

int main(void) {
    printf("Initializing...");
    if (mma8x5x_init(&dev, &mma8x5x_params[0]) != 0) {
        printf("Failed to initialize MMA8x5x sensor\n");
        return 1;
    }

    printf("MMA8x5x sensor initialized\n");
    while (1) {
        read_acceleration();
        xtimer_sleep(INTERVAL);
    }

    return 0;
}