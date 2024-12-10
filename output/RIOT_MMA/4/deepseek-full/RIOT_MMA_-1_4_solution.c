
#include "mma8x5x.h"
#include "unistd.h"
#include "dht_params.h"
#include "periph/rtc.h"
#include "xtimer.h"
#include "assert.h"
#include "event/periodic_callback.h"
#include "stdlib.h"
#include "mma8x5x_params.h"
#include "timex.h"
#include "periph/i2c.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "stdio.h"
#include "evtimer_mbox.h"
#include "ztimer.h"
#include "event/thread.h"
#include "thread.h"
#include "time_units.h"
#include "string.h"
#include "freertos/FreeRTOS.h"
#include "mma8x5x_regs.h"
#include "kernel_defines.h"
#include "periph/gpio.h"

#define MMA8X5X_I2C_ADDRESS CONFIG_MMA8X5X_I2C_ADDRESS

static mma8x5x_t dev;
static mma8x5x_data_t data;

static void read_accelerometer(void) {
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

    while (1) {
        read_accelerometer();
        vTaskDelay(pdMS_TO_TICKS(5000)); // Delay for 5 seconds
    }

    return 0;
}
