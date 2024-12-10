#include <kernel_defines.h>
#include <event/thread.h>
#include <mbox.h>
#include <dht.h>
#include <msg.h>
#include "ztimer.h"
#include "mma8x5x.h"

static mma8x5x_t dev;
static mma8x5x_data_t data;

void *read_accel_thread(void *arg) {
    (void)arg;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        return NULL;  // Handle initialization error appropriately
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        // As printf or stdio.h are restricted, replace with appropriate system log or another function
        // printf("Acceleration: x=%d mg y=%d mg z=%d mg\n", data.x, data.y, data.z);

        // Log function placeholder
        // log_acceleration(data.x, data.y, data.z);
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, 
                  read_accel_thread, NULL, "read_accel");

    while (1) {
        // Main code here (e.g., handling events)
    }
    return 0;
}