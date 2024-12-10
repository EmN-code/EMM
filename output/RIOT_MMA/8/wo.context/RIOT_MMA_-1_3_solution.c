#include <timex.h>
#include <thread.h>
#include <stdio.h>
#include <stdlib.h>
#include "mma8x5x.h"
#include "mma8x5x_params.h"

static mma8x5x_t dev;
static mma8x5x_data_t data;
static kernel_pid_t main_pid;

void *mma8x5x_thread(void *arg) {
    (void)arg;
    while(1) {
        if (mma8x5x_read(&dev, &data)) {
            printf("X: %d, Y: %d, Z: %d\n", data.x_axis, data.y_axis, data.z_axis);
        }
        else {
            printf("Error reading data from MMA8x5x\n");
        }
        ztimer_sleep(ZTIMER_SEC, 5); // sleep for 5 seconds
    }
    return NULL;
}

int main(void)
{
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS, // Corrected macro name
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }

    char stack[THREAD_STACKSIZE_MAIN];
    main_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                             THREAD_CREATE_STACKTEST, mma8x5x_thread, NULL, "mma8x5x");

    return 0;
}