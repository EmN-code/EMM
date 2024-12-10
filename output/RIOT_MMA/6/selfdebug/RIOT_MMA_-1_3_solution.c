#include "ztimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"

static mma8x5x_t dev;
static mma8x5x_data_t data;

void read_mma8x5x(void) {
    mma8x5x_read(&dev, &data);
    printf("X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
}

int main(void) {
    printf("RIOT: MMA8x5x example with ESP32\n");

    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }

    ztimer_timer_t timer;
    ztimer_set_msg(&timer, ZTIMER_MSEC, 5000, NULL, read_mma8x5x);

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 5000);
        read_mma8x5x();
    }

    return 0;
}