#include <xtimer.h>
#include <periph/i2c.h>
#include <kernel_defines.h>

#include "mma8x5x.h"

#define I2C_ADDR          0x1D  // MMA8x5x default I2C address
#define I2C_BUS           I2C_DEV(0)
#define RATE              MMA8X5X_RATE_50HZ
#define RANGE             MMA8X5X_RANGE_2G

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_BUS,
    .addr = I2C_ADDR,
    .rate = RATE,
    .range = RANGE
};

void init_mma8x5x(void) {
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Initialization failed\n");
        return;
    }
    printf("Initialization successful\n");
}

void read_sensor_values(void) {
    mma8x5x_data_t data;
    if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
        printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
    } else {
        printf("Failed to read sensor values\n");
    }
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();
    init_mma8x5x();

    while (1) {
        read_sensor_values();
        xtimer_periodic_wakeup(&last_wakeup, 5 * US_PER_SEC);
    }

    return 0;
}