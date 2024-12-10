#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "architecture.h"
#include "periph_conf.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "mma8x5x_params.h"
#include "dht_params.h"

#define SLEEP_TIME (5U * US_PER_SEC)

static mma8x5x_t dev;
mma8x5x_data_t data;
mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = 0x1D,  /* Example I2C address for MMA8x5x, verify actual address */
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

static void read_accelerometer(void)
{
    if (mma8x5x_read(&dev, &data) != MMA8X5X_OK) {
        printf("Failed to read accelerometer data\n");
    }
    else {
        printf("Acceleration X: %d mg, Y: %d mg, Z: %d mg\n",
                    data.x_axis, data.y_axis, data.z_axis);
    }
}

int main(void)
{
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize accelerometer\n");
        return 1;
    }
    while (1) {
        read_accelerometer();
        ztimer_sleep(ZTIMER_SEC, 5);
    }
    return 0;
}