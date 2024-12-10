#include <xtimer.h>
#include <periph/i2c.h>
#include <mma8x5x/mma8x5x.h>

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void)
{
    puts("MMA8x5x example on ESP32");

    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization failed");
        return 1;
    }

    while (1) {
        if (mma8x5x_read(&dev, &data) != MMA8X5X_OK) {
            puts("Failed to read data");
            continue;
        }
        printf("X: %d, Y: %d, Z: %d\n", data.acc[0], data.acc[1], data.acc[2]);
        xtimer_sleep(5);
    }

    return 0;
}