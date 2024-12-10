
#include <stdio.h>
#include <ztimer.h>
#include <msg.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <timex.h>

#define MSG_QUEUE_SIZE 8

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mma8x5x_t dev;
static mma8x5x_data_t data;

void event_loop(void) {
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        mma8x5x_read(&dev, &data);
        printf("Acceleration: X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
    }
}

int main(void) {
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

    thread_t *event_thread = thread_create(NULL, 512, THREAD_PRIORITY_MAIN - 1,
                                           THREAD_CREATE_STACKTEST, event_loop, NULL, "event_loop");

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 5000);
        msg_send(&msg_queue[0], event_thread);
    }

    return 0;
}
