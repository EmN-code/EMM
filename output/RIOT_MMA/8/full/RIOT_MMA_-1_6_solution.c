#include <ztimer.h>
#include <xtimer.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "msg.h"
#include "kernel_defines.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "arpa/inet.h"
#include "sys/socket.h"
#include "unistd.h"
#include "thread.h"
#include "evtimer_msg.h"
#include "mma8x5x_regs.h"
#include "timex.h"
#include "event/thread.h"
#include "dht_params.h"
#include "paho_mqtt.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "stdatomic.h"
#include "netinet/in.h"
#include "architecture.h"
#include "mbox.h"
#include "evtimer_mbox.h"

#define SLEEP_TIME (5U)

static mma8x5x_t dev;
static mma8x5x_data_t data;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void) {
    int res;

    // Initialize the MMA8x5x device
    res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Failed to initialize accelerometer: %d\n", res);
        return 1;
    }

    // Loop to read accelerometer data every 5 seconds
    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration X: %d mg Y: %d mg Z: %d mg\n",
               data.x, data.y, data.z);

        // Sleep for the defined interval
        ztimer_sleep(ZTIMER_SEC, SLEEP_TIME);
    }

    return 0;
}