
#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "msg.h"
#include "kernel_defines.h"
#include "mbox.h"
#include "thread.h"
#include "event/thread.h"
#include "mqtt-sn.h"
#include "MQTTClient.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "dht_params.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "time_units.h"

#define CONFIG_MMA8X5X_I2C_ADDRESS  0x1D  // Define the I2C address

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    // Initialize the MMA8x5x device
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Accelerometer data: x=%d, y=%d, z=%d\n", data.x, data.y, data.z);  // Assuming x, y, z are the correct members; update if needed.
        xtimer_sleep(5);
    }

    return 0;
}