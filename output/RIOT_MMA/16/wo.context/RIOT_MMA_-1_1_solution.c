#include <thread.h>
#include <stdio.h>
#include <mma8x5x.h>
#include <xtimer.h>

#define INTERVAL (5U * 1000000U)  // 5 seconds in microseconds

static void *sensor_thread(void *arg) {
    mma8x5x_t dev;
    mma8x5x_params_t params = {
        .params = 0  // Assume default initialization parameters
    };

    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Failed to initialize MMA8X5X sensor\n");
        return NULL;
    }

    while (1) {
        if (mma8x5x_is_ready(&dev)) {
            // Here you would typically read from the sensor
            printf("Sensor ready to provide data\n");
        } else {
            printf("Sensor not ready\n");
        }

        xtimer_sleep(5); // Sleep for 5 seconds
    }

    return NULL;
}

int main(void) {
    puts("Starting MMA8X5X reader thread\n");

    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0,
                  sensor_thread, NULL, "sensor_thread");

    return 0;
}