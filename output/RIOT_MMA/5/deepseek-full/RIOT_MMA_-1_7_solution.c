
#include "mma8x5x.h"
#include "unistd.h"
#include "esp_wifi.h"
#include "dht_params.h"
#include "arpa/inet.h"
#include "periph/rtc.h"
#include "xtimer.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "assert.h"
#include "stddef.h"
#include "event/periodic_callback.h"
#include "stdlib.h"
#include "net/sock/udp.h"
#include "mma8x5x_params.h"
#include "timex.h"
#include "periph/i2c.h"
#include "mbox.h"
#include "net/sock/util.h"
#include "evtimer_msg.h"
#include "freertos/task.h"
#include "paho_mqtt.h"
#include "stdio.h"
#include "evtimer_mbox.h"
#include "MQTTClient.h"
#include "netinet/in.h"
#include "ztimer.h"
#include "event/thread.h"
#include "thread.h"
#include "time_units.h"
#include "string.h"
#include "freertos/FreeRTOS.h"
#include "esp_system.h"
#include "sched.h"
#include "architecture.h"
#include "nvs_flash.h"
#include "mma8x5x_regs.h"
#include "kernel_defines.h"
#include "msg.h"
#include "sys/socket.h"
#include "periph/gpio.h"
#include "stdbool.h"

#define INTERVAL_SEC (5U)

static mma8x5x_t dev;
static mma8x5x_data_t data;

static void read_accelerometer(void) {
    mma8x5x_read(&dev, &data);
    printf("Accelerometer data: x=%d, y=%d, z=%d\n", data.x, data.y, data.z);
}

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Failed to initialize MMA8X5X accelerometer\n");
        return 1;
    }

    while (1) {
        read_accelerometer();
        sleep(INTERVAL_SEC);
    }

    return 0;
}
