
#include "paho_mqtt.h"
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "msg.h"
#include "dht.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "kernel_defines.h"
#include "netinet/in.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "dht_params.h"
#include "stdlib.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "arpa/inet.h"
#include "thread.h"
#include "mma8x5x_regs.h"
#include "unistd.h"
#include "stdatomic.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "mma8x5x_params.h"

#define STACK_SIZE (1024)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define INTERVAL (5 * US_PER_SEC)  // 5 seconds interval

static char stack[STACK_SIZE];
static mma8x5x_t dev;
static mma8x5x_data_t data;

static void read_sensor(void) {
    mma8x5x_read(&dev, &data);
    printf("Acceleration: x: %d, y: %d, z: %d\n", data.x, data.y, data.z);
}

static void sensor_thread(void *arg) {
    (void)arg;
    while (1) {
        read_sensor();
        ztimer_sleep(ZTIMER_MSEC, INTERVAL);
    }
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
        printf("Failed to initialize MMA8x5x accelerometer\n");
        return 1;
    }

    thread_create(stack, sizeof(stack), PRIORITY, 0, sensor_thread, NULL, "sensor_thread");

    return 0;
}
