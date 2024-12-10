#include <thread.h> // Include first according to alphabetical order
#include <periph_conf.h> // Includes configuration for peripherals like timers
#include <xtimer.h> // To provide timer functionality
#include <stdio.h> // For input and output operations
#include <mma8x5x.h> // Header for interfacing with mma8x5x sensor

#define INTERVAL (5000000U) // 5 seconds in microseconds

static xtimer_ticks32_t last_wakeup;
static mma8x5x_t dev;
static const mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_ADDR,
};

void *read_mma8x5x(void *arg) {
    (void)arg;
    mma8x5x_data_t data;
    
    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);

        if (mma8x5x_read(&dev, &data) == 0) {
            printf("MMA8x5x data: x=%d, y=%d, z=%d\n", data.x, data.y, data.z);
        } else {
            printf("Failed to read MMA8x5x data\n");
        }
    }

    return NULL;
}

int main(void) {
    printf("Initializing MMA8x5x sensor\n");
    if (mma8x5x_init(&dev, &params) == 0) {
        printf("MMA8x5x sensor initialized successfully\n");
    } else {
        printf("Failed to initialize MMA8x5x sensor\n");
        return 1;
    }

    last_wakeup = xtimer_now();

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, read_mma8x5x, NULL, "mma8x5x_reader");

    return 0;
}