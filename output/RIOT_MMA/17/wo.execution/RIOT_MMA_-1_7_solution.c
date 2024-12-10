#include "ztimer.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdio.h"
#include "stdbool.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "msg.h"
#include "mbox.h"
#include "unistd.h"
#include "thread_arch.h"
#include "timex.h"
#include "thread.h"
#include "time_units.h"
#include "paho_mqtt.h"
#include "kernel_defines.h"
#include "architecture.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "MQTTClient.h"

static char stack[THREAD_STACKSIZE_MAIN];

static void *accel_thread(void *arg) {
    mma8x5x_t *dev = (mma8x5x_t *)arg;
    mma8x5x_data_t data;

    while (1) {
        if (mma8x5x_is_ready(dev)) {
            // Reading accelerometer data
            mma8x5x_read(dev, &data);
            // Print out the data as placeholder since the specifics of data struct were not provided
            printf("Accelerometer data: x=%d y=%d z=%d\n", data.x_axis, data.y_axis, data.z_axis);
        }
        // Delay for 5 seconds
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    return NULL;
}

int main(void) {
    // Initialize device params
    mma8x5x_params_t params;
    params.i2c = I2C_DEV(0); // I2C bus device
    params.addr = CONFIG_MMA8X5X_I2C_ADDRESS; // Assuming proper configuration macro set up
    params.rate = MMA8X5X_RATE_50HZ;
    params.range = MMA8X5X_RANGE_2G;

    mma8x5x_t dev;

    // Initialize sensor
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Failed to initialize accelerometer");
        return 1;
    }

    // Create thread to read accelerometer data
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  accel_thread, &dev, "accel_thread");

    return 0;
}
