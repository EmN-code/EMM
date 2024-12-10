#include "ztimer.h"
#include "xtimer.h"
#include "periph_conf.h"
#include "periph_cpu.h"
#include "periph/i2c.h"
#include "cpu.h"
#include "mma8x5x.h"

#define TIMER_INTERVAL     (5U * MS_PER_SEC)

// Initialize device descriptor and params
static mma8x5x_t dev;
static mma8x5x_data_t data;
static const mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

// Function to read accelerometer data
static void _read_mma8x5x(void) {
    if (mma8x5x_init(&dev, &params) == MMA8X5X_OK) {
        mma8x5x_read(&dev, &data);
        printf("X: %dmg, Y: %dmg, Z: %dmg\n", data.x_axis, data.y_axis, data.z_axis);
    } else {
        printf("Failed to initialize mma8x5x\n");
    }
}

int main(void) {
    ztimer_sleep(ZTIMER_MSEC, TIMER_INTERVAL);  // Wait 5 seconds interval
    while (1) {
        _read_mma8x5x();
        ztimer_sleep(ZTIMER_MSEC, TIMER_INTERVAL);
    }
    return 0;
}