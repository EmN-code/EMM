#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "periph_conf.h"
#include "mma8x5x.h"
#include "thread.h"

#define INTERVAL (5U)  // Interval in seconds

static mma8x5x_t dev;
static const mma8x5x_params_t params = {
    .params = {/* initialize needed parameters for MMA8x5x sensor here */},
};

static void read_sensor(void) {
    int16_t x, y, z;

    if (mma8x5x_read(&dev, &x, &y, &z) != 0) {
        printf("Error reading sensor\n");
        return;
    }

    printf("Acceleration [x]: %d [y]: %d [z]: %d\n", x, y, z);
}

void *thread_function(void *arg) {
    (void)arg;

    while (true) {
        read_sensor();
        thread_sleep_s(INTERVAL);
    }

    return NULL;
}

int main(void) {
    printf("Starting MMA8x5x sensor reading on ESP32\n");

    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Error initializing sensor\n");
        return 1;
    }

    kernel_pid_t thread_pid = thread_create(
        thread_stack, sizeof(thread_stack),
        THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
        thread_function, NULL, "mma8x5x_reader"
    );

    if (thread_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating thread\n");
        return 1;
    }

    // Keep main thread alive to maintain the program
    while (true) {
        thread_sleep();
    }

    return 0;
}