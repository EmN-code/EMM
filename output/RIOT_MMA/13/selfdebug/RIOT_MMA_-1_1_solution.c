#include "thread.h"
#include "periph/timer.h"
#include "mma8x5x.h"

#define TIMER_INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds

static mma8x5x_t sensor;
static mma8x5x_params_t params = {
    // Sensor parameters like address, bus, scaling mode, etc. can be set here
    // These will need to be filled out based on your specific sensor configuration.
};

static void _mma8x5x_read(void) {
    int16_t x, y, z;
    if (mma8x5x_read(&sensor, &x, &y, &z) == 0) {
        printf("x: %d, y: %d, z: %d\n", x, y, z);
    } else {
        puts("Error reading accelerometer!");
    }
}

static void _timer_cb(void *arg) {
    (void)arg;
    _mma8x5x_read();
}

int main(void) {
    puts("MMA8x5x sensor application");

    // Initialize the sensor
    if (mma8x5x_init(&sensor, &params) != 0) {
        puts("Sensor initialization failed!");
        return 1;
    }

    // Setup timer
    tim_t dev = TIMER_DEV(0);
    timer_init(dev, TIMER_FREQ_1MHZ, _timer_cb, NULL);
    timer_set(dev, TIMER_INTERVAL);
    timer_start(dev);

    while (1) {
        thread_yield();
    }

    return 0;
}