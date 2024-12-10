#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "time_units.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "event/thread.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define WAIT_TIME (5U * 1000U * 1000U) // 5 seconds in microseconds

static mma8x5x_t dev;
static mma8x5x_data_t data;
mma8x5x_params_t params = {
    .i2c = 0,  // Assuming I2C_DEV(0) corresponds to 0
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void read_sensor(void) {
    if (mma8x5x_is_ready(&dev)) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration: x=%d mg, y=%d mg, z=%d mg\n",
               data.x_axis, data.y_axis, data.z_axis);
    } else {
        printf("Sensor is not ready.\n");
    }
}

int main(void) {
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize accelerometer.\n");
        return 1;
    }

    ztimer_sleep(ZTIMER_USEC, WAIT_TIME);  // sleep for initial set-up

    while (1) {
        read_sensor();
        ztimer_sleep(ZTIMER_USEC, WAIT_TIME);  // wait for 5 seconds
    }

    return 0;
}