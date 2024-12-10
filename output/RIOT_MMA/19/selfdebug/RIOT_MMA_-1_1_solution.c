#include <thread.h>
#include <periph_conf.h>
#include <kernel.h>
#include <stdio.h>
#include <mma8x5x.h>

// setup the MMA8x5x parameters
static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_ADDR_DEF,
    .range = MMA8X5X_RANGE_2G,
    .data_rate = MMA8X5X_DATA_RATE_100HZ
};

void* mma8x5x_read_thread(void *arg) {
    (void)arg;
    int16_t x, y, z;

    while (1) {
        if (mma8x5x_read(&dev, &x, &y, &z) != 0) {
            printf("Failed to read data from MMA8X5X device\n");
        } else {
            printf("x: %d, y: %d, z: %d\n", x, y, z);
        }
        // sleep for 5 seconds
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    puts("RIOT MMA8x5x Test\n");

    // Initialize the device
    if (mma8x5x_init(&dev, &params) != 0) {
        puts("Failed to initialize MMA8X5X device\n");
        return 1;
    }

    // Create a thread to read the MMA8X5X sensor
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mma8x5x_read_thread, NULL, "mma8x5x_reader");

    // Everything is done by threads
    thread_sleep();
    return 0;
}