
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stddef.h>
#include <msg.h>
#include <thread.h>
#include <ztimer.h>
#include <paho_mqtt.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static mma8x5x_t dev;
static mma8x5x_data_t data;

static void *accelerometer_thread(void *arg)
{
    (void)arg;

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration: X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return NULL;
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

    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x accelerometer\n");
        return 1;
    }

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, accelerometer_thread, NULL, "accelerometer");

    return 0;
}
