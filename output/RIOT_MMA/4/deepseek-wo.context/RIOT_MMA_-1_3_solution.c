
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "ztimer.h"
#include "thread.h"
#include "time_units.h"
#include "timex.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static mma8x5x_t dev;
static mma8x5x_data_t data;

void read_accelerometer(void)
{
    mma8x5x_read(&dev, &data);
    printf("Accelerometer data: x = %d, y = %d, z = %d\n", data.x, data.y, data.z);
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x accelerometer\n");
        return 1;
    }

    while (1) {
        read_accelerometer();
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return 0;
}
