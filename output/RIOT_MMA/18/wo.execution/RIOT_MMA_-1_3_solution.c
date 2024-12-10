#include "time_units.h"
#include "unistd.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "periph_conf.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "dht_params.h"
#include "event/thread.h"
#include "thread.h"

#define CONFIG_MMA8X5X_I2C_ADDRESS 0x1D

static mma8x5x_t dev;
mma8x5x_data_t data;
mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void *read_accelerometer(void *arg) {
    (void)arg;  // Unused parameter
    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration X: %d mg, Y: %d mg, Z: %d mg\n", data.x_axis, data.y_axis, data.z_axis);
        usleep(5000000); // Sleep for 5 seconds
    }
    return NULL;
}

int main(void) {
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("MMA8x5x: Initialization failed!");
        return 1;
    }

    char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  read_accelerometer, NULL, "accelerometer");

    return 0;
}