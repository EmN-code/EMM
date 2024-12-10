#include <thread.h>
#include <stdio.h>
#include <xtimer.h>
#include <mma8x5x.h>

#define INTERVAL (5U * 1000000U) // 5 seconds in microseconds

static mma8x5x_t dev;

void *read_sensor(void *arg) {
    (void)arg;
    while (1) {
        // Reading data from the sensor
        if (mma8x5x_is_ready(&dev)) {
            printf("MMA8x5x sensor is ready.\n");
        } else {
            printf("MMA8x5x sensor is not ready.\n");
        }
        
        // Sleep for the defined interval
        xtimer_usleep(INTERVAL);
    }
    return NULL;
}

int main(void) {
    // Initialize the sensor with default parameters
    mma8x5x_params_t params;
    mma8x5x_init(&dev, &params);
    
    // Create a new thread to read sensor data
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, read_sensor, NULL, "sensor");

    // The main thread does nothing but wait
    while (1) {
        thread_yield();
    }
    return 0;
}