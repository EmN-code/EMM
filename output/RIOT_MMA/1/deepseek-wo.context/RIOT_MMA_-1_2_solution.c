
#include "evtimer_msg.h"
#include "event/thread.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "ztimer.h"
#include "mma8x5x.h"
#include "xtimer.h"
#include "evtimer_mbox.h"

#define MMA8X5X_I2C_ADDRESS CONFIG_MMA8X5X_I2C_ADDRESS
#define MMA8X5X_RATE_1_56HZ MMA8X5X_RATE_50HZ

static mma8x5x_t dev;
static mma8x5x_data_t data;

static void read_accelerometer(void) {
    mma8x5x_read(&dev, &data);
    printf("Accelerometer data: x=%d, y=%d, z=%d\n", data.x, data.y, data.z);
}

static void event_callback(void *arg) {
    (void)arg;
    read_accelerometer();
}

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x accelerometer\n");
        return 1;
    }

    evtimer_t event_timer;
    evtimer_init_msg(&event_timer);

    while (1) {
        evtimer_add_msg(&event_timer, ZTIMER_SEC, event_callback, NULL);
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return 0;
}
