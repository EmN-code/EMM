#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "msg.h"
#include "mbox.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "dht_params.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "event/thread.h"
#include "MQTTClient.h"
#include "time_units.h"

#define SLEEP_INTERVAL (5U * US_PER_SEC)

static mma8x5x_t dev;
static mma8x5x_data_t data;
static mma8x5x_params_t params = {
    .i2c = 0, // change to actual I2C dev id, if it's not 0
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void *sensor_reading_thread(void *arg) {
    (void)arg;
    int status;
    while (1) {
        status = mma8x5x_init(&dev, &params);
        if (status == MMA8X5X_OK) {
            break;
        }
        else {
            printf("Failed to initialize sensor! Retrying...\n");
            xtimer_sleep(1);
        }
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration: x=%d mg y=%d mg z=%d mg\n", data.x, data.y, data.z);
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return NULL;
}

int main(void) {
    thread_create(
        sensor_reading_thread_stack, sizeof(sensor_reading_thread_stack),
        THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
        sensor_reading_thread, NULL, "sensor_reading_thread");

    // Main thread can do other tasks or go to sleep.
    while (1) {
        xtimer_sleep(60);
    }

    return 0;
}

static char sensor_reading_thread_stack[THREAD_STACKSIZE_MAIN];