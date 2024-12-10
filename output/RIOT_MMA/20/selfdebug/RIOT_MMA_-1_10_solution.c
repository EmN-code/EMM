#include <ztimer.h>
#include <periph/i2c.h>
#include "mma8x5x.h"

static mma8x5x_t dev;
static mma8x5x_data_t data;

void initialize_sensor(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        // Handle initialization error
    }
}

void read_sensor_data(void) {
    mma8x5x_read(&dev, &data);
    // Process or display data as needed
}

int main(void) {
    initialize_sensor();
    
    while (1) {
        read_sensor_data();
        ztimer_sleep(ZTIMER_SEC, 5);
    }
    return 0;
}