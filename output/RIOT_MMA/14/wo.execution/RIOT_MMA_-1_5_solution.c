#include <timex.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <unistd.h>
#include <dht_params.h>
#include <dht.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <kernel_defines.h>
#include <mbox.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <MQTTClient.h>
#include <msg.h>
#include <periph_conf.h>
#include <periph/gpio.h>
#include <periph/i2c.h>
#include <periph/rtc.h>
#include <paho_mqtt.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <thread.h>
#include <time_units.h>
#include <timex.h>
#include <unistd.h>
#include <ztimer.h>

#define SLEEP_INTERVAL (5U * 1000000U)  // Sleep for 5 seconds in microseconds

static mma8x5x_t dev;
static mma8x5x_data_t data;

void *read_accelerometer(void *arg) {
    (void)arg;
    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration x: %d y: %d z: %d\n", data.x, data.y, data.z);
        ztimer_sleep_usec(ZTIMER_USEC, SLEEP_INTERVAL);
    }
    return NULL;
}

int main(void) {
    const mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Failed to initialize the accelerometer.");
        return 1;
    }

    thread_create(NULL, THREAD_STACKSIZE_MAIN, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, read_accelerometer, NULL, "accel_read_thread");

    return 0;
}