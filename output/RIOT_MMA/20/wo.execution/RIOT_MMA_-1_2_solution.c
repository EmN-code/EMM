#include "thread_config.h"
#include "thread.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph_conf.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "periph/gpio.h"
#include "event/thread.h"

// Assuming that the printf function is available
#include <stdio.h>

static mma8x5x_t dev;
static mma8x5x_data_t data;
mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void *read_sensor(void *arg) {
    (void)arg;
    while (1) {
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        } else {
            printf("Failed to read data\n");
        }
        thread_sleep(5);
    }
    return NULL;
}

int main(void) {
    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Error: Could not initialize MMA8x5x (Error Code %d)\n", res);
        return 1;
    }
    printf("MMA8x5x initialized successfully\n");

    thread_create(NULL, 1024, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, read_sensor, NULL, "sensor_reader");

    return 0;
}