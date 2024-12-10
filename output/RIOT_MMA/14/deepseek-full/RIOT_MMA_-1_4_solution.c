
#include <ztimer.h>
#include <stddef.h>
#include <stdbool.h>
#include <msg.h>
#include <dht.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <stdlib.h>
#include <mbox.h>
#include <stdio.h>
#include <xtimer.h>
#include <mma8x5x_regs.h>
#include <unistd.h>
#include <stdatomic.h>
#include <mma8x5x.h>
#include <dht_params.h>
#include <mma8x5x_params.h>

#define MSG_QUEUE_SIZE 16
#define SENSOR_READ_INTERVAL (5 * US_PER_SEC)

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mma8x5x_t dev;
static mma8x5x_data_t data;

void sensor_read_task(void)
{
    while (1) {
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("Accelerometer data: x=%d, y=%d, z=%d\n", data.x, data.y, data.z);
        } else {
            printf("Failed to read accelerometer data\n");
        }
        ztimer_sleep(ZTIMER_SEC, SENSOR_READ_INTERVAL / US_PER_SEC);
    }
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

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

    printf("MMA8x5x accelerometer initialized successfully\n");

    sensor_read_task();

    return 0;
}
