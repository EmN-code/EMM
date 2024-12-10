#include "unistd.h"
#include "time_units.h"
#include "thread.h"
#include "stddef.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "periph/rtc.h"
#include "periph_conf.h"
#include "stdio.h"
#include "mma8x5x.h"
#include "stdatomic.h"
#include "dht_params.h"
#include "event/thread.h"
#include "MQTTClient.h"

#define SLEEP_DURATION (5 * US_PER_SEC)
#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)

static char stack[THREAD_STACKSIZE_MAIN];

static void *mma8x5x_thread(void *arg) {
    mma8x5x_t *dev = (mma8x5x_t *)arg;
    mma8x5x_data_t data;
    while (1) {
        mma8x5x_read(dev, &data);
        printf("Acceleration data: X=%dmg, Y=%dmg, Z=%dmg\n", data.x, data.y, data.z);
        usleep(SLEEP_DURATION);
    }
    return NULL;
}

int main(void) {
    mma8x5x_t dev;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };
    
    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Initialization failed\n");
        return 1;
    }

    thread_create(stack,
                  sizeof(stack),
                  THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST,
                  mma8x5x_thread,
                  &dev,
                  "mma8x5x_sensor");

    while (1) {
        // main thread can perform other tasks or enter low-power mode
    }

    return 0;
}