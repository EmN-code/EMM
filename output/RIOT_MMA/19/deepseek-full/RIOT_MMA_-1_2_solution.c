
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "event/thread.h"
#include "mma8x5x_params.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "xtimer.h"

#define MAIN_QUEUE_SIZE     (8)

static mma8x5x_t dev;
static mma8x5x_data_t data;

static void *main_thread(void *arg)
{
    (void)arg;

    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Initialization failed\n");
        return NULL;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration [mg]: X: %d Y: %d Z: %d\n", data.x, data.y, data.z);
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void)
{
    event_queue_t main_queue;
    event_queue_init(&main_queue);

    thread_create(
        stack, sizeof(stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        main_thread,
        NULL, "main_thread");

    return 0;
}
