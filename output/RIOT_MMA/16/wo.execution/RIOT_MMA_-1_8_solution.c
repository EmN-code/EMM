#include "ztimer.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"
#include "stddef.h"
#include "stdatomic.h"
#include "periph_conf.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "netinet/in.h"
#include "arfa/inet.h"
#include "unistd.h"
#include "sys/socket.h"
#include "msg.h"
#include "timex.h"
#include "thread.h"
#include "mutex.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "mbox.h"
#include "time_units.h"
#include "mutex.h"
#include "dht_params.h"
#include "dht.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "kernel_defines.h"

#define I2C_DEV            I2C_DEV(0)
#define MMA8X5X_I2C_ADDRESS 0x1D

static mma8x5x_t dev;
static mma8x5x_data_t data;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV,
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void *read_mma8x5x(void *arg) {
    (void)arg;
    if (mma8x5x_init(&dev, &params) == MMA8X5X_OK) {
        puts("MMA8x5x initialized successfully");
    }
    else {
        puts("MMA8x5x initialization failed");
    }
    while (1) {
        mma8x5x_read(&dev, &data);
        printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, read_mma8x5x, NULL, "mma8x5x");
    return 0;
}