#include "xtimer.h"
#include "thread.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "msg.h"
#include "mma8x5x.h"

#define INTERVAL (5U * US_PER_SEC)

static void _init_mma8x5x(mma8x5x_t *dev) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(dev, &params) != MMA8X5X_OK) {
        puts("Failed to initialize MMA8x5x");
    }
}

static void _read_mma8x5x(const mma8x5x_t *dev) {
    mma8x5x_data_t data;
    mma8x5x_read(dev, &data);
    printf("Accel X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
}

static void *mma8x5x_thread(void *arg) {
    mma8x5x_t *dev = (mma8x5x_t *)arg;

    while (1) {
        xtimer_sleep(INTERVAL);
        _read_mma8x5x(dev);
    }
    return NULL;
}

int main(void) {
    static mma8x5x_t dev;
    _init_mma8x5x(&dev);
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mma8x5x_thread,
                  &dev, "mma8x5x_thread");
    return 0;
}