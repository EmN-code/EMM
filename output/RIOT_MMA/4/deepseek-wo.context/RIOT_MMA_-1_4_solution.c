
#include "mma8x5x.h"
#include "periph/i2c.h"
#include "msg.h"
#include "thread.h"
#include "time_units.h"

#define MMA8X5X_I2C_ADDRESS CONFIG_MMA8X5X_I2C_ADDRESS

static mma8x5x_t dev;
static msg_t msg_queue[8];

void read_accelerometer(void) {
    mma8x5x_data_t data;
    mma8x5x_read(&dev, &data);
    printf("Accelerometer data: x=%d, y=%d, z=%d\n", data.x, data.y, data.z);
}

int main(void) {
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

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
        vtimer_sleep(5 * US_PER_SEC);
    }

    return 0;
}