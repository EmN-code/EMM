#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdio.h>
#include <time_units.h>
#include <timex.h>
#include <thread.h>
#include <ztimer.h>
#include <xtimer.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <dht.h>
#include <dht_params.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <kernel_defines.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>

#define MAIN_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static void *sensor_thread(void *arg) {
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
        printf("Error: Unable to initialize the MMA8x5x sensor.\n");
        return NULL;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    thread_create(stack,
                  MAIN_THREAD_STACKSIZE,
                  THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST,
                  sensor_thread,
                  NULL,
                  "sensor_thread");

    while (1) {
        xtimer_sleep(1000);
    }

    return 0;
}