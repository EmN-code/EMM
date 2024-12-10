#include <ztimer.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stddef.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <unistd.h>
#include <timex.h>
#include <thread.h>
#include <time_units.h>
#include <stdio.h>
#include <mbox.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <event/thread.h>
#include <kernel_defines.h>
#include <dht_params.h>
#include <evtimer_msg.h>
#include <stdbool.h>
#include <evtimer_mbox.h>

static mma8x5x_t dev;
static mma8x5x_data_t data;

static const mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void read_mma8x5x(void) {
    mma8x5x_read(&dev, &data);
    printf("Acceleration data: x=%d mg, y=%d mg, z=%d mg\n", data.x, data.y, data.z);
}

int main(void) {
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }

    while (1) {
        read_mma8x5x();
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}