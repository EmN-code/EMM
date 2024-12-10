
#include <stddef.h>
#include <dht_params.h>
#include <arpa/inet.h>
#include <mma8x5x.h>
#include <evtimer_mbox.h>
#include <net/emcute.h>
#include <thread.h>
#include <net/gcoap.h>
#include <kernel_defines.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <string.h>
#include <time_units.h>
#include <evtimer_msg.h>
#include <event/thread.h>
#include <mbox.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <periph/rtc.h>
#include <timex.h>
#include <stdio.h>
#include <periph/i2c.h>
#include <stdbool.h>
#include <msg.h>
#include <ztimer.h>
#include <event/periodic_callback.h>
#include <netinet/in.h>
#include <periph/gpio.h>
#include <net/sock/util.h>

#define SLEEP_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void)
{
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("MMA8x5x accelerometer initialization failed\n");
        return 1;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration: X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}
