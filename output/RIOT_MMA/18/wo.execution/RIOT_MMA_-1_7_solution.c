#include "ztimer.h"
#include "xtimer.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stddef.h"
#include "stdbool.h"
#include "paho_mqtt.h"
#include "periph_conf.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "thread.h"
#include "unistd.h"
#include "time_units.h"

static mma8x5x_t dev;
static mma8x5x_data_t data;
mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void *mma8x5x_thread(void *arg) {
    (void)arg;
    int result = mma8x5x_init(&dev, &params);
    if (result != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x sensor\n");
        return NULL;
    }
    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration: x: %d mg, y: %d mg, z: %d mg\n",
                data.x, data.y, data.z);
        ztimer_sleep(ZTIMER_SEC, 5);
    }
    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  mma8x5x_thread, NULL, "mma8x5x_thread");
    return 0;
}
