
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <periph/rtc.h>
#include <timex.h>
#include <thread.h>
#include <mbox.h>
#include <event/periodic_callback.h>
#include <net/emcute.h>
#include <kernel_defines.h>
#include <time_units.h>
#include <dht_params.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <xtimer.h>
#include <ztimer.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>

#define MMA8X5X_I2C_ADDRESS (0x1D)
#define MMA8X5X_RATE_50HZ   (0x0A)
#define MMA8X5X_RANGE_2G    (0x00)

static mma8x5x_t dev;
static mma8x5x_data_t data;

static void periodic_read_accelerometer(void) {
    mma8x5x_read(&dev, &data);
    printf("Accelerometer data: x=%d, y=%d, z=%d\n", data.x, data.y, data.z);
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

    periodic_callback_t periodic;
    periodic_callback_init(&periodic, xtimer_ticks32(xtimer_s_to_ticks(5)), periodic_read_accelerometer);
    periodic_callback_start(&periodic);

    while (1) {
        periodic_callback_await(&periodic);
    }

    return 0;
}
