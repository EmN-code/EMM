#include "unistd.h"
#include "timex.h"
#include "thread.h"
#include "periph_conf.h"
#include "periph/i2c.h"
#include "mma8x5x.h"
#include "mma8x5x_regs.h"
#include "ztimer.h"
#include "xtimer.h"
#include "msg.h"
#include "mbox.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "dht_params.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "dht.h"

#define I2C_ADDR        (MMA8X5X_I2C_ADDRESS)
#define I2C_FREQ        (MMA8X5X_RATE_50HZ)
#define MMA8X5X_RANGE   (MMA8X5X_RANGE_2G)
#define SLEEP_INTERVAL  (5U * US_PER_SEC)  // 5 seconds

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = I2C_ADDR,
    .rate = I2C_FREQ,
    .range = MMA8X5X_RANGE
};

void read_sensor_data(void)
{
    mma8x5x_data_t data;
    mma8x5x_read(&dev, &data);
    printf("Acceleration x: %d mg, y: %d mg, z: %d mg\n", data.x, data.y, data.z);
}

void *sensor_thread(void *arg)
{
    (void)arg;
    while (1) {
        read_sensor_data();
        xtimer_sleep(SLEEP_INTERVAL / US_PER_SEC);
    }
    return NULL;
}

int main(void)
{
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize the accelerometer\n");
        return -1;
    }

    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, sensor_thread, NULL, "sensor_thread");

    return 0;
}