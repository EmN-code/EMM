
#include "xtimer.h"
#include "mma8x5x.h"
#include "periph/gpio.h"

#define SLEEP_SECONDS 5

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .mode = MMA8X5X_MODE_NORMAL,
    .sample_rate = MMA8X5X_RATE_1_56HZ,
    .scale = MMA8X5X_SCALE_2G
};

void print_acceleration(mma8x5x_t *dev) {
    mma8x5x_data_t data;
    if (mma8x5x_read(dev, &data) == 0) {
        printf("Acceleration: X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
    } else {
        printf("Failed to read acceleration data\n");
    }
}

int main(void) {
    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Failed to initialize MMA8x5x sensor\n");
        return 1;
    }

    while (1) {
        print_acceleration(&dev);
        xtimer_sleep(SLEEP_SECONDS);
    }

    return 0;
}
