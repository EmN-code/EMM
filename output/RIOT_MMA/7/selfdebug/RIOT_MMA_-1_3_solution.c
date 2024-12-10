#include "mma8x5x.h"
#include "kernel_defines.h"
#include "kernel_types.h"
#include "msg.h"
#include <stdio.h>

#define SLEEP_TIME (5U)  // sleep time in seconds

static kernel_pid_t _main_thread_pid;

static void read_sensor_data(void) {
    mma8x5x_t dev;
    mma8x5x_data_t data;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x sensor.\n");
        return;
    }

    while (1) {
        if (mma8x5x_read(&dev, &data) != 0) {
            printf("Error reading MMA8x5x sensor data.\n");
        } else {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        }
        xtimer_sleep(SLEEP_TIME);
    }
}

int main(void) {
    _main_thread_pid = thread_getpid();
    msg_init_queue(main_mb, MAIN_QUEUE_SIZE);
    read_sensor_data();
    return 0;
}