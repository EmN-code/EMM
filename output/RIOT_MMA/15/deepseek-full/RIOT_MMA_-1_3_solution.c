
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "ztimer.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} mma8x5x_data_t;

typedef struct {
    i2c_t i2c;
    uint8_t addr;
    uint8_t rate;
    uint8_t range;
} mma8x5x_params_t;

typedef struct {
    mma8x5x_params_t params;
    uint8_t initialized;
} mma8x5x_t;

static mma8x5x_t dev;

int mma8x5x_init(mma8x5x_t *dev, const mma8x5x_params_t *params) {
    // Dummy implementation for initialization
    dev->params = *params;
    dev->initialized = 1;
    return 0;
}

void mma8x5x_read(const mma8x5x_t *dev, mma8x5x_data_t *data) {
    // Dummy implementation for reading data
    data->x = 100;
    data->y = 200;
    data->z = 300;
}

void event_loop(void) {
    mma8x5x_data_t data;
    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration: X=%d, Y=%d, Z=%d\n", data.x, data.y, data.z);
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = 0x1C,
        .rate = 50,
        .range = 2
    };

    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Initialization failed\n");
        return 1;
    }

    event_loop();

    return 0;
}
