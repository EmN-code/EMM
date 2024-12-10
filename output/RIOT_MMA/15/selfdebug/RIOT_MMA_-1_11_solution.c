#include "ztimer.h"
#include "xtimer.h"
#include "periph/i2c.h"
#include "net/ble.h"
#include "mma8x5x.h"

static mma8x5x_t dev;
static mma8x5x_data_t data;

void mma8x5x_read_task(void)
{
    // Read data from the MMA8x5x accelerometer
    mma8x5x_read(&dev, &data);
    printf("Acceleration data: X=%d, Y=%d, Z=%d (mg)\n", data.x, data.y, data.z);
}

int main(void)
{
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x!\n");
        return 1;
    }

    ztimer_sleep(ZTIMER_SEC, 5);

    while (1) {
        mma8x5x_read_task();
        ztimer_periodic_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}