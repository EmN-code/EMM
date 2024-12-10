
#include <stdio.h>
#include <unistd.h>
#include "ztimer.h"
#include "msg.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static mma8x5x_t dev;
static mma8x5x_data_t data;

void read_accelerometer(void) {
    mma8x5x_read(&dev, &data);
    printf("Accelerometer data: X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,  // Corrected from MMA8X5X_I2C_ADDRESS
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x accelerometer\n");
        return 1;
    }

    while (1) {
        read_accelerometer();
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return 0;
}
