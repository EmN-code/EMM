#include "ztimer.h"
#include "xtimer.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "stddef.h"
#include "periph/i2c.h"
#include "msg.h"
#include "thread.h"
#include "kernel_defines.h"
#include "arpa/inet.h"
#include "periph/rtc.h"
#include "time_units.h"
#include "net/sock/udp.h"
#include "MQTTClient.h"
#include "evtimer_msg.h"
#include "dht_params.h"
#include "dht.h"
#include "net/sock/util.h"
#include "periph/gpio.h"
#include "net/gcoap.h"
#include "paho_mqtt.h"
#include "mbox.h"
#include "event/thread.h"
#include "net/emcute.h"
#include "timex.h"
#include "netinet/in.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "evtimer_mbox.h"
#include "periph_conf.h"
#include "mma8x5x_regs.h"

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

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return NULL;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Accel data: X=%d mg, Y=%d mg, Z=%d mg\n", data.x, data.y, data.z);
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, read_mma8x5x, NULL, "read_mma");

    return 0;
}