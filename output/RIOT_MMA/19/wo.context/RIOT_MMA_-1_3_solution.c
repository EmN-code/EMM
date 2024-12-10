#include "xtimer.h"
#include "ztimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"
#include "stdio.h"

#define I2C_ADDR        0x1D
#define I2C_DEV         I2C_DEV(0)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static mma8x5x_t dev;
static mma8x5x_data_t data;

static void read_accelerometer(void)
{
    if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
        printf("Acceleration data - X: %d, Y: %d, Z: %d\n", data.x_axis, data.y_axis, data.z_axis);
    }
    else {
        printf("Failed to read acceleration data.\n");
    }
}

int main(void)
{
    mma8x5x_params_t params;
    params.i2c = I2C_DEV;
    params.addr = I2C_ADDR;
    params.rate = MMA8X5X_RATE_50HZ;
    params.range = MMA8X5X_RANGE_2G;

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x.\n");
        return 1;
    }

    while (1) {
        read_accelerometer();
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}