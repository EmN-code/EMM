#include "ztimer.h"
#include "xtimer.h"
#include "stdio.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "math.h"
#include "esp_system.h"

#define THREAD_STACKSIZE_MAIN (1024)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)

static char stack[THREAD_STACKSIZE_MAIN];
static kernel_pid_t thread_pid;

static void *read_mma8x5x(void *arg) {
    (void)arg;
    mma8x5x_t dev;
    mma8x5x_data_t data;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Error initializing MMA8x5x\n");
        return NULL;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration: x=%d mg, y=%d mg, z=%d mg\n", data.x_axis, data.y_axis, data.z_axis);
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN,
                               THREAD_CREATE_STACKTEST, read_mma8x5x, NULL, "mma8x5x_reader");
    if (thread_pid < 0) {
        printf("Error creating thread\n");
    }
    return 0;
}