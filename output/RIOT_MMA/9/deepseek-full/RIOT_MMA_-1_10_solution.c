
#include "net/gcoap.h"
#include "paho_mqtt.h"
#include "esp_system.h"
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "mutex.h"
#include "msg.h"
#include "dht.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "kernel_defines.h"
#include "netinet/in.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "net/sock/util.h"
#include "MQTTClient.h"
#include "stdlib.h"
#include "net/sock/udp.h"
#include "sys/socket.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "arpa/inet.h"
#include "thread.h"
#include "mma8x5x_regs.h"
#include "unistd.h"
#include "stdatomic.h"
#include "net/emcute.h"
#include "mma8x5x.h"
#include "esp_wifi.h"
#include "dht_params.h"
#include "mma8x5x_params.h"

#define STACKSIZE 1024
#define PRIORITY 5

static char stack[STACKSIZE];
static msg_t msg_queue[8];

void accelerometer_thread(void *arg)
{
    (void)arg;
    mma8x5x_t dev;
    mma8x5x_data_t data;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Accelerometer initialization failed");
        return;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration: X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        ztimer_sleep(ZTIMER_SEC, 5);
    }
}

int main(void)
{
    msg_init_queue(msg_queue, 8);

    thread_create(stack, STACKSIZE, PRIORITY, 0, accelerometer_thread, NULL, "accelerometer_thread");

    return 0;
}
