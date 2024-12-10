
#include "xtimer.h"
#include "mma8x5x.h"
#include "periph/gpio.h"

#define SLEEP_INTERVAL (5 * 1000 * 1000U)  // 5 seconds in microseconds

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .mode = MMA8X5X_MODE_NORMAL,
    .sample_rate = MMA8X5X_RATE_1_56HZ,
    .scale = MMA8X5X_SCALE_2G
};

void read_acceleration(void) {
    int16_t x, y, z;
    if (mma8x5x_read(&dev, &x, &y, &z) == 0) {
        printf("Acceleration: X=%d, Y=%d, Z=%d\n", x, y, z);
    } else {
        puts("Failed to read acceleration data");
    }
}

int main(void) {
    // Initialize the MMA8x5x sensor
    if (mma8x5x_init(&dev, &params) != 0) {
        puts("Initialization of MMA8x5x sensor failed");
        return 1;
    }

    while (1) {
        read_acceleration();
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}
