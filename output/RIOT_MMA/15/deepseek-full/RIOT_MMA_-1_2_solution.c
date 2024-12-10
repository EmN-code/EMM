
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "event/thread.h"
#include "mma8x5x_params.h"

#define MAIN_QUEUE_SIZE     (8)

static mma8x5x_t dev;
static mma8x5x_data_t data;

static void *event_loop(void *arg)
{
    (void)arg;
    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration: X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
        vtimer_sleep(5); // Sleep for 5 seconds
    }
    return NULL;
}

int main(void)
{
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Initialization failed with error code %d\n", res);
        return 1;
    }

    thread_create(NULL,
                  THREAD_STACKSIZE_DEFAULT,
                  THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST,
                  event_loop,
                  NULL,
                  "event_loop");

    return 0;
}
