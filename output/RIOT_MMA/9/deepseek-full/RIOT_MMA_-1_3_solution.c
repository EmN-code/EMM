
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <xtimer.h>
#include <msg.h>
#include <mbox.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <timex.h>
#include <ztimer.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <dht_params.h>

#define MMA8X5X_I2C_ADDRESS 0x1D
#define MMA8X5X_RATE_50HZ   0x04
#define MMA8X5X_RANGE_2G    0x00

static mma8x5x_t dev;
static mma8x5x_data_t data;
static mbox_t mbox;
static msg_t msg_queue[8];

void read_sensor(void) {
    if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
        printf("Accelerometer data: x=%d, y=%d, z=%d\n", data.x, data.y, data.z);
    } else {
        printf("Failed to read accelerometer data\n");
    }
}

void timer_callback(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 0;
    msg_send(&msg, mbox_fd(&mbox));
}

int main(void) {
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

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

    evtimer_t timer;
    evtimer_init(&timer);
    evtimer_add(&timer, &timer_callback, NULL, TIME_S_TO_TICKS(5));

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        read_sensor();
    }

    return 0;
}
