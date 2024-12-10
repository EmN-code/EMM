#include "periph/i2c.h"
#include "msg.h"
#include "time_units.h"
#include "periph/rtc.h"
#include "thread.h"
#include "arpa/inet.h"
#include "mutex.h"
#include "stdbool.h"
#include "kernel_defines.h"
#include "net/sock/udp.h"
#include "stdio.h"
#include "MQTTClient.h"
#include "evtimer_msg.h"
#include "dht_params.h"
#include "sys/socket.h"
#include "net/sock/util.h"
#include "xtimer.h"
#include "dht.h"
#include "periph/gpio.h"
#include "net/gcoap.h"
#include "paho_mqtt.h"
#include "ztimer.h"
#include "stdlib.h"
#include "stddef.h"
#include "mbox.h"
#include "event/thread.h"
#include "net/emcute.h"
#include "timex.h"
#include "netinet/in.h"
#include "mma8x5x.h"
#include "unistd.h"
#include "mma8x5x_params.h"
#include "evtimer_mbox.h"
#include "periph_conf.h"
#include "esp_system.h"
#include "mma8x5x_regs.h"
#include "stdatomic.h"

#define THREAD_STACKSIZE_MAIN 2048
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)

void *mma8x5x_thread(void *arg) {
    mma8x5x_t *dev = (mma8x5x_t *)arg;
    mma8x5x_data_t data;

    while(1) {
        mma8x5x_read(dev, &data);
        printf("x: %d mg\ty: %d mg\tz: %d mg\n", data.x, data.y, data.z);
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    puts("RIOT MMA8x5x example");

    mma8x5x_t dev;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    int init_res = mma8x5x_init(&dev, &params);

    if (init_res != MMA8X5X_OK) {
        printf("Error initializing MMA8x5x\n");
        return 1;
    }

    char thread_stack[THREAD_STACKSIZE_MAIN];
    thread_create(thread_stack, THREAD_STACKSIZE_MAIN, THREAD_PRIORITY_MAIN, 0,
                  mma8x5x_thread, &dev, "mma8x5x_thread");

    return 0;
}
