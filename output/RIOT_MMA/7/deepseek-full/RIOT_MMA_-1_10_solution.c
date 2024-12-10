
#include <stdio.h>
#include <unistd.h>
#include "msg.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "paho_mqtt.h"
#include "thread.h"

#define STACKSIZE 1024
#define PRIORITY 5
#define MSG_QUEUE_SIZE 8

static mma8x5x_t dev;
static msg_t msg_queue[MSG_QUEUE_SIZE];

void accelerometer_thread(void *arg) {
    (void)arg;
    mma8x5x_data_t data;

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Accelerometer data: x=%d, y=%d, z=%d\n", data.x, data.y, data.z);
        sleep(5);
    }
}

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("MMA8x5x initialization failed: %d\n", res);
        return 1;
    }

    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    char stack[STACKSIZE];
    thread_create(stack, STACKSIZE, PRIORITY, 0, accelerometer_thread, NULL, "accelerometer_thread");

    return 0;
}
