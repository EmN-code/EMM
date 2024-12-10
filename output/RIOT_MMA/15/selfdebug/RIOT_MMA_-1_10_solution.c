#include "ztimer.h"
#include "mma8x5x.h"
#include "periph/i2c.h"

static mma8x5x_t dev;
static mma8x5x_data_t data;

static const mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = CONFIG_MMA8X5X_I2C_ADDRESS, // Corrected constant name
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

static void read_acc_data(void) {
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Error: Unable to initialize MMA8x5x\n");
        return;
    }
    if (mma8x5x_read(&dev, &data) != 0) {
        printf("Error: Unable to read data from MMA8x5x\n");
        return;
    }

    printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
}

int main(void) {
    ztimer_t timer;
    ztimer_set(&timer, ZTIMER_MSEC_BASE, 5 * 1000); // Correct period as 'ZTIMER_MSEC_BASE'
    while (1) {
        if (ztimer_now(ZTIMER_MSEC_BASE) >= ztimer_read(&timer)) {
            read_acc_data();
            ztimer_set(&timer, ZTIMER_MSEC_BASE, 5 * 1000);
        }
        ztimer_periodic_sleep(ZTIMER_MSEC_BASE, 500); // Sleep to save CPU
    }
    return 0;
}
