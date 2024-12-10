
#include "mma8x5x.h"
#include "mma8x5x_regs.h"
#include "evtimer_mbox.h"
#include "ztimer.h"
#include "mma8x5x_params.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "xtimer.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static mma8x5x_t dev;
static mma8x5x_data_t data;

static void _read_sensor(void)
{
    mma8x5x_read(&dev, &data);
    printf("Acceleration: X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
}

static void _event_callback(void *arg)
{
    (void)arg;
    _read_sensor();
    evtimer_add_msg(0, _event_callback, NULL);
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Initialization failed\n");
        return 1;
    }

    evtimer_add_msg(0, _event_callback, NULL);

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}
