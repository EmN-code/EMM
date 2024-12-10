#include "xtimer.h"
#include "stdio.h"
#include "net/emcute.h"
#include "kernel_defines.h"
#include "architecture.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "msg.h"
#include "MQTTClient.h"
#include "ztimer.h"
#include "ztimer.h"
#include "xtimer.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "arpa/inet.h"
#include "time_units.h"
#include "unint.h"

static mma8x5x_t dev;
mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void *accel_read_loop(void *arg) {
    (void)arg;
    while (1) {
        mma8x5x_data_t data;
        if (mma8x5x_is_ready(&dev)) {
            mma8x5x_read(&dev, &data);
            printf("X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
        } else {
            printf("Accelerometer not ready\n");
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    if (mma8x5x_init(&dev, &params) == MMA8X5X_OK) {
        printf("MMA8x5x initialized\n");
    } else {
        printf("MMA8x5x initialization failed\n");
        return 1;
    }
    
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, accel_read_loop, NULL, "accelerometer");
    
    return 0;
}