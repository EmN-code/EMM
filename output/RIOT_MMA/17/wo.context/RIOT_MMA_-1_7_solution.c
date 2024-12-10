#include "xtimer.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x.h"

#define MMA8X5X_I2C_ADDRESS 0x1D
#define READ_INTERVAL (5U)

int main(void) {
    mma8x5x_t dev;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    mma8x5x_data_t data;
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization failed!");
        return 1;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration data [mg]: X= %i Y= %i Z= %i\n", data.x, data.y, data.z);
        xtimer_sleep(READ_INTERVAL);
    }

    return 0;
}