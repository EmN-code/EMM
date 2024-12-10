#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "msg.h"
#include "mbox.h"
#include "event/thread.h"
#include "MQTTClient.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "dht_params.h"

#define SLEEP_INTERVAL (5U) // 5 seconds interval

static mma8x5x_t dev;
static mma8x5x_data_t data;

static const mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void)
{
    // Initialize accelerometer
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("[Error] Unable to initialize MMA8x5x\n");
        return 1;
    }

    while (1) {
        // Read accelerometer data
        mma8x5x_read(&dev, &data);
        printf("Acceleration data [mg]: x=%d y=%d z=%d\n", data.x, data.y, data.z);
        
        // Sleep for specified interval
        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}