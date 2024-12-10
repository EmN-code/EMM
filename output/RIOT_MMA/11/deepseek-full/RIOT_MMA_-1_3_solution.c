
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <msg.h>
#include <mbox.h>
#include <timex.h>
#include <xtimer.h>
#include <ztimer.h>
#include <evtimer_mbox.h>

#define MMA8X5X_I2C_ADDRESS 0x1D
#define MMA8X5X_RATE_50HZ 0x04
#define MMA8X5X_RANGE_2G 0x00

typedef struct {
    i2c_t i2c;
    uint8_t addr;
    uint8_t rate;
    uint8_t range;
} mma8x5x_params_t;

typedef struct {
    mma8x5x_params_t params;
    i2c_t i2c;
    uint8_t addr;
} mma8x5x_t;

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} mma8x5x_data_t;

static mma8x5x_t dev;
static mbox_t mbox;
static msg_t msg_queue[8];

int mma8x5x_init(mma8x5x_t *dev, const mma8x5x_params_t *params) {
    // Dummy implementation for initialization
    return 0;
}

void read_accelerometer(void) {
    mma8x5x_data_t data;
    // Dummy implementation for reading accelerometer data
    printf("Accelerometer data: x=%d, y=%d, z=%d\n", data.x, data.y, data.z);
}

void periodic_read(void *arg) {
    (void)arg;
    read_accelerometer();
    xtimer_sleep(5);
}

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Failed to initialize MMA8x5x accelerometer\n");
        return 1;
    }

    evtimer_mbox_add_msg(&mbox, (evtimer_mbox_msg_t){periodic_read, NULL}, NULL);

    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        ((void (*)(void *))msg.content.ptr)(msg.content.ptr);
    }

    return 0;
}
