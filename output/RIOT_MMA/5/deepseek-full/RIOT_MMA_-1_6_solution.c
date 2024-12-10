
#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "periph/i2c.h"
#include "periph/rtc.h"
#include "xtimer.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "event/periodic_callback.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "freertos/task.h"
#include "freertos/FreeRTOS.h"
#include "architecture.h"
#include "mma8x5x_regs.h"
#include "kernel_defines.h"
#include "msg.h"
#include "sys/socket.h"
#include "periph/gpio.h"
#include "timex.h"
#include "time_units.h"
#include "dht_params.h"
#include "ztimer.h"
#include "event/thread.h"
#include "thread.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "netinet/in.h"

#define SLEEP_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static mma8x5x_t dev;
static mma8x5x_data_t data;

static void read_accelerometer(void) {
    mma8x5x_read(&dev, &data);
    printf("Accelerometer data: x=%d, y=%d, z=%d\n", data.x, data.y, data.z);
}

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x accelerometer: %d\n", res);
        return 1;
    }

    while (1) {
        read_accelerometer();
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}
