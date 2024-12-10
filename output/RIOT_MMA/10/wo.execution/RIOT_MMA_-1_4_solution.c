#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "thread.h"
#include "timex.h"
#include "msg.h"
#include "mbox.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "dht_params.h"
#include "MQTTClient.h"

#include <stdio.h>

#define CONFIG_MMA8X5X_I2C_ADDRESS 0x1D  // Correctly define the I2C address

static mma8x5x_t dev;
mma8x5x_data_t data;

void read_accel(void) {
    // Check if device is ready
    if (mma8x5x_is_ready(&dev)) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration [mg]: X=%d, Y=%d, Z=%d\n", data.acc[0], data.acc[1], data.acc[2]);
    } else {
        printf("Device not ready\n");
    }
}

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };
    
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Error initializing accelerometer\n");
        return 1;
    }

    while (1) {
        read_accel();
        ztimer_sleep(ZTIMER_MSEC, 5000); // Sleep for 5 seconds
    }

    return 0;
}