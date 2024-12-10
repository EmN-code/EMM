#include "xtimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = CONFIG_MMA8X5X_I2C_ADDRESS, // Make sure this constant is defined in your config
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};
mma8x5x_data_t data;

void read_sensor(void) {
    if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
        printf("Acceleration [X,Y,Z]: %d, %d, %d\n", 
               data.acc[0], 
               data.acc[1], 
               data.acc[2]);
    } else {
        puts("Failed to read acceleration data");
    }
}

int main(void) {
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Failed to initialize the accelerometer");
        return 1;
    }

    while (1) {
        read_sensor();
        xtimer_sleep(5); // Sleep for 5 seconds
    }

    return 0;
}