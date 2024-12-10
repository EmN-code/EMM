#include <unistd.h>
#include <ztimer.h>
#include <xtimer.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <thread.h>
#include <timex.h>
#include <time_units.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdio.h>
#include <paho_mqtt.h>
#include <periph_conf.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <periph/rtc.h>
#include <msg.h>
#include <kernel_defines.h>
#include <mbox.h>
#include <dht_params.h>
#include <dht.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>

static mma8x5x_t dev;
static mma8x5x_data_t data;

// Device parameters
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void *sensor_loop(void *arg) {
    (void)arg;

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Error initializing accelerometer\n");
        return NULL;
    }

    while(1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration: X=%d mg, Y=%d mg, Z=%d mg\n", data.x_axis, data.y_axis, data.z_axis);

        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return NULL;
}

int main(void) {
    thread_create(NULL, THREAD_STACKSIZE_MAIN, THREAD_PRIORITY_MAIN - 1, 0, sensor_loop, NULL, "sensor_loop");
    
    return 0;
}