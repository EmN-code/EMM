#include <ztimer.h>
#include <xtimer.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdatomic.h>
#include <unistd.h>
#include <string.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <netinet/in.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <msg.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <time_units.h>
#include <thread.h>
#include <kernel_defines.h>
#include <arpa/inet.h>
#include <event/thread.h>
#include <mbox.h>
#include <paho_mqtt.h>
#include <dht_params.h>
#include <dht.h>
#include <MQTTClient.h>

#define SLEEP_INTERVAL (5U)

static mma8x5x_t dev;
static mma8x5x_data_t data;
mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void read_accel(void)
{
    if (mma8x5x_init(&dev, &params) == MMA8X5X_OK) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
    }
    else {
        printf("Failed to initialize MMA8x5x!\n");
    }
}

int main(void)
{
    puts("MMA8x5x acceleration reader\n");

    while (1) {
        read_accel();
        ztimer_sleep(ZTIMER_MSEC, SLEEP_INTERVAL * 1000);
    }

    return 0;
}