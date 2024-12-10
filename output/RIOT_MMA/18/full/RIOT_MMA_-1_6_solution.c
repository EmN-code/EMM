#include "ztimer.h"
#include "xtimer.h"
#include "thread.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"
#include "unistd.h"
#include "netinet/in.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "arpa/inet.h"
#include "paho_mqtt.h"
#include "net/emcute.h"
#include "dht.h"
#include "dht_params.h"
#include "kernel_defines.h"
#include "net/gcoap.h"
#include "msg.h"
#include "mma8x5x.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "MQTTClient.h"
#include "mbox.h"
#include "timex.h"
#include "time_units.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"

static mma8x5x_t dev;
static mma8x5x_data_t data;
static const mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void *read_sensor(void *arg) {
    (void)arg;
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Initialization failed\n");
        return NULL;
    }
    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Accel: x=%d, y=%d, z=%d\n", data.x, data.y, data.z);
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    thread_create(NULL, THREAD_STACKSIZE_MAIN, THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST,
                  read_sensor, NULL, "read_sensor");
    return 0;
}