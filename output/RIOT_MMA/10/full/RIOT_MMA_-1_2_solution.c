#include "thread_arch.h"
#include "thread.h"
#include "periph_rtc.h"
#include "periph_i2c.h"
#include "periph_gpio.h"
#include "periph_conf.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "event_thread.h"

#include <stdio.h>

static char thread_stack[THREAD_STACKSIZE_DEFAULT];

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
        printf("Failed to initialize MMA8x5x\n");
        return NULL;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration: x=%d, y=%d, z=%d\n", data.x, data.y, data.z);
        thread_sleep(5);
    }

    return NULL;
}

int main(void) {
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, read_mma8x5x, NULL, "mma_thread");

    return 0;
}