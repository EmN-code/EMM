#include "periph_conf.h"
#include "periph/i2c.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "event/thread.h"
#include "periph/gpio.h"
#include "dht_params.h"
#include "periph/rtc.h"
#include "mma8x5x_params.h"
#include "thread.h"
#include "MQTTClient.h"

#define THREAD_STACKSIZE_MAIN  (THREAD_STACKSIZE_DEFAULT)

static char stack[THREAD_STACKSIZE_MAIN];
static mma8x5x_t dev;
static mma8x5x_data_t data;

void *read_accelerometer(void *arg) {
    (void) arg;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        return NULL;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration: x=%d, y=%d, z=%d\n", data.x, data.y, data.z);
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    return NULL;
}

int main(void) {
    printf("RIOT accelerometer example\n");
    int flags = 0;
    if (thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, flags,
                      read_accelerometer, NULL, "accelerometer_thread") <= KERNEL_PID_UNDEF) {
        printf("Error creating thread\n");
        return 1;
    }
    return 0;
}