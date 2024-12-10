#include "netinet/in.h"
#include "assert.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "esp_wifi.h"
#include "dht_params.h"
#include "xtimer.h"
#include "msg.h"
#include "mbox.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "ztimer.h"
#include "stdatomic.h"
#include "mma8x5x.h"
#include "stdio.h"
#include "esp_system.h"
#include "mma8x5x_regs.h"
#include "stdbool.h"
#include "mma8x5x_params.h"
#include "stddef.h"
#include "thread.h"
#include "time_units.h"
#include "dht.h"
#include "net/emcute.h"
#include "unistd.h"
#include "net/sock/udp.h"
#include "stdlib.h"
#include "evtimer_msg.h"
#include "mutex.h"
#include "freertos/FreeRTOS.h"
#include "MQTTClient.h"
#include "sys/types.h"
#include "freertos/task.h"
#include "event/thread.h"
#include "paho_mqtt.h"
#include "timex.h"
#include "evtimer_mbox.h"
#include "kernel_defines.h"

#define SLEEP_TIME_S (5U)

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    // Configuration parameters for the accelerometer
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    // Initialize the MMA8x5x accelerometer
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Initialization failed\n");
        return 1;
    }

    while (1) {
        // Read accelerometer data
        mma8x5x_read(&dev, &data);
        printf("X: %d, Y: %d, Z: %d\n", data.x_axis, data.y_axis, data.z_axis);

        // Sleep for the specified duration
        xtimer_sleep(SLEEP_TIME_S);
    }
    return 0;
}