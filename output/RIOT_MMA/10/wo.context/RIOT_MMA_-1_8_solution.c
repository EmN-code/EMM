#include <ztimer.h>
#include <periph/i2c.h>
#include <mma8x5x.h>

static mma8x5x_t dev;
static mma8x5x_data_t data;

static void read_mma8x5x(void *arg) {
    if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
        printf("Accel Data - x: %d, y: %d, z: %d\n", data.x, data.y, data.z);
    }
    ztimer_t *timer = (ztimer_t *)arg;
    ztimer_set(timer, ZTIMER_SEC, 5);
}

int main(void) {
    ztimer_t timer;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization failed!");
        return 1;
    }

    timer.target = ZTIMER_SEC_NOW + 5;
    timer.callback = read_mma8x5x;
    timer.arg = &timer;
    ztimer_set(&timer);
    return 0;
}