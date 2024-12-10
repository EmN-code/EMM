#include "ztimer.h"
#include "timex.h"
#include "xtimer.h"
#include "thread.h"
#include "msg.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "time_units.h"
#include "mma8x5x_regs.h"
#include "dht_params.h"
#include "evtimer_msg.h"
#include "mbox.h"
#include "evtimer_mbox.h"

#define INTERVAL 5

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void read_accelerometer(void)
{
    int16_t acc[3];
    if (mma8x5x_read(&dev, acc) == MMA8X5X_OK) {
        printf("Acceleration [mg]: X=%d Y=%d Z=%d\n", acc[0], acc[1], acc[2]);
    }
    else {
        printf("Failed to read accelerometer data\n");
    }
}

int main(void)
{
    msg_init_queue(_main_msg_queue, ARRAY_SIZE(_main_msg_queue));

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize accelerometer\n");
        return 1;
    }

    while (1) {
        read_accelerometer();
        ztimer_sleep(ZTIMER_SEC, INTERVAL);
    }
    return 0;
}