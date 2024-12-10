#include "xtimer.h"
#include "thread.h"
#include "time_units.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "event/thread.h"

#define SLEEP_INTERVAL   (5U)

static mma8x5x_t dev;
static const mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

static void *read_mma8x5x(void *arg) {
    (void)arg;
    mma8x5x_data_t data;
    
    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
        xtimer_sleep(SLEEP_INTERVAL);
    }
    return NULL;
}

int main(void) {
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Initialization of accelerometer failed\n");
        return 1;
    }
    
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, read_mma8x5x, NULL, "mma8x5x thread");

    return 0;
}