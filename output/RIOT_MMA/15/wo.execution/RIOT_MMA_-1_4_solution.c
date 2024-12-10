#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "time_units.h"
#include "thread.h"
#include "timex.h"
#include "msg.h"
#include "mbox.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "paho_mqtt.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "dht_params.h"
#include "dht.h"
#include "MQTTClient.h"

#include <stdio.h>

static mma8x5x_t dev;
static mma8x5x_data_t data;
static const mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void *read_mma8x5x(void *arg) {
    (void)arg;
    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Failed to initialize mma8x5x sensor\n");
        return NULL;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Accel X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
        ztimer_sleep(ZTIMER_MSEC, 5000); // Sleep for 5 seconds
    }

    return NULL;
}

int main(void) {
    printf("Starting MMA8x5x accelerometer test\n");
    char thread_stack[THREAD_STACKSIZE_MAIN];
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, read_mma8x5x, NULL, "mma8x5x");
    return 0;
}