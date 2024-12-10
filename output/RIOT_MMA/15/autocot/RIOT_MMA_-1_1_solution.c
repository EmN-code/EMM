#include <thread.h>
#include <periph_conf.h>
#include <mma8x5x.h>

mma8x5x_t dev;
tim_t timer_dev = TIMER_DEV(0);
uint32_t timer_interval_us = 5000000; // 5 seconds

void timer_callback(void *arg) {
    (void)arg;
    if (mma8x5x_is_ready(&dev)) {
        // Read sensor data
        int16_t x, y, z;
        mma8x5x_acquire(&dev);
        mma8x5x_read_accel(&dev, &x, &y, &z);
        mma8x5x_release(&dev);
        printf("Accelerometer values: x=%d, y=%d, z=%d\n", x, y, z);
    }
}

int main(void) {
    printf("Initializing MMA8x5x sensor...\n");
    mma8x5x_params_t params;
    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Sensor initialization failed.\n");
        return 1;
    }

    // Initialize the timer
    timer_init(timer_dev, timer_interval_us, timer_callback, NULL);

    // Start the timer
    timer_set(timer_dev, timer_interval_us);

    while (1) {
        thread_yield();
    }
    return 0;
}