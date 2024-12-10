#include "periph/i2c.h"
#include "thread.h"
#include "xtimer.h"
#include "mma8x5x.h"

static mma8x5x_t dev;
static mma8x5x_data_t data;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void *read_accelerometer(void *arg) {
    (void) arg;
    while (1) {
        mma8x5x_read(&dev, &data);
        printf("X: %d | Y: %d | Z: %d\n", data.x, data.y, data.z);
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to init MMA8x5x\n");
        return 1;
    }

    char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  read_accelerometer, NULL, "read_accel");

    return 0;
}