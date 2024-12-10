#define ENABLE_DEBUG (1)
#include "kernel_defines.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stdio.h"
#include "unistd.h"
#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "msg.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "mbox.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "periph_conf.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "dht.h"
#include "dht_params.h"
#include "time_units.h"
#include "stdio.h"
#include "rtc.h"

// Define I2C device and address based on configuration
#ifndef CONFIG_MMA8X5X_I2C_ADDRESS
#define CONFIG_MMA8X5X_I2C_ADDRESS 0x1D // Default I2C address
#endif
#define I2C_DEV(i)       (i)

static char stack[THREAD_STACKSIZE_DEFAULT];
static mma8x5x_t dev;
static mma8x5x_data_t data;

static void *read_mma8x5x(void *arg) {
    (void)arg;
    while(1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration data: x = %d mg, y = %d mg, z = %d mg\n", data.x_axis, data.y_axis, data.z_axis);
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    return NULL;
}

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Initialization failed\n");
        return 1;
    }

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, read_mma8x5x, NULL, "mma8x5x_thread");

    return 0;
}