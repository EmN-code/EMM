#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <string.h>
#include "xtimer.h"
#include "ztimer.h"
#include "msg.h"
#include "thread.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "mma8x5x.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "dht.h"
#include "dht_params.h"
#include "time_units.h"
#include "unistd.h"
#include "mqtt_client.h"
#include "paho_mqtt.h"

#define I2C_DEV(0) 0  // Define I2C device macro to prevent undeclared error.
#define CONFIG_MMA8X5X_I2C_ADDRESS 0x1D  // Placeholder I2C address

static mma8x5x_t dev;
static mma8x5x_data_t data;
mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = CONFIG_MMA8X5X_I2C_ADDRESS,  // Correct address variable here
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void *thread_handler(void *arg) {
    (void)arg;
    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration: X=%d mg, Y=%d mg, Z=%d mg\n", data.x, data.y, data.z);
        xtimer_sleep(5);  // Sleep for 5 seconds before the next read
    }
    return NULL;
}

int main(void) {
    int result = mma8x5x_init(&dev, &params);
    if (result != MMA8X5X_OK) {
        printf("Failed to initialize accelerometer! Error code: %d\n", result);
    }

    char stack[THREAD_STACKSIZE_MAIN];

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  thread_handler, NULL, "mma8x5x_reader");

    /* Start empty loop; thread_handler does the rest */
    while (1) {
        ztimer_sleep(ZTIMER_USEC, 1);  // You can use ztimer for microsecond accuracy if needed
    }

    return 0;
}