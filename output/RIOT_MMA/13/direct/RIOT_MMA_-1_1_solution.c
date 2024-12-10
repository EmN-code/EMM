#include <xtimer.h>
#include <stdio.h>
#include <mma8x5x.h>

#define SLEEP_INTERVAL (5U * US_PER_SEC)

mma8x5x_t dev;
mma8x5x_params_t params = MMA8X5X_PARAMS_DEFAULT;

void read_sensor(void) {
    int16_t acc[3];
    if (mma8x5x_read(&dev, acc) != 0) {
        puts("Failed to read from sensor");
        return;
    }
    printf("Acceleration - X: %d, Y: %d, Z: %d\n", acc[0], acc[1], acc[2]);
}

int main(void) {
    printf("Initializing sensor...\n");
    if (mma8x5x_init(&dev, &params) != 0) {
        puts("Failed to initialize sensor");
        return 1;
    }

    while (1) {
        read_sensor();
        xtimer_sleep(SLEEP_INTERVAL / US_PER_SEC);
    }

    return 0;
}