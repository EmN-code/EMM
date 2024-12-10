#include "mma8x5x.h"
#include "msg.h"
#include "kernel.h"
#include "thread.h"
#include "xtimer.h"

typedef struct {
    mma8x5x_params_t params;
    mma8x5x_t dev;
    mma8x5x_data_t data;
} mma8x5x_device_t;

static mma8x5x_device_t mma8x5x_device;

#define MMA8X5X_I2C_ADDRESS (0x1D)
#define SAMPLE_RATE         (5U)        /* in Hz */
#define SLEEP_INTERVAL      (5U * US_PER_SEC)

void *mma8x5x_thread(void *arg) {
    while (1) {
        mma8x5x_read(&mma8x5x_device.dev, &mma8x5x_device.data);
        printf("X: %d, Y: %d, Z: %d\n", mma8x5x_device.data.x, mma8x5x_device.data.y, mma8x5x_device.data.z);
        xtimer_sleep(SLEEP_INTERVAL);
    }
    return NULL;
}

int main(void) {
    puts("Initializing MMA8x5x sensor...");

    /* initialize device parameters */
    mma8x5x_device.params.i2c = I2C_DEV(0);
    mma8x5x_device.params.addr = MMA8X5X_I2C_ADDRESS;
    mma8x5x_device.params.rate = SAMPLE_RATE;
    mma8x5x_device.params.range = MMA8X5X_RANGE_2G;

    /* initialize MMA8x5x sensor */
    if (mma8x5x_init(&mma8x5x_device.dev, &mma8x5x_device.params) != MMA8X5X_OK) {
        puts("Initialization failed!");
        return 1;
    }

    /* create a thread to read accelerometer data */
    char mma8x5x_thread_stack[THREAD_STACKSIZE_MAIN];
    thread_create(mma8x5x_thread_stack, sizeof(mma8x5x_thread_stack), THREAD_PRIORITY_MAIN-1, THREAD_CREATE_STACKTEST, mma8x5x_thread, NULL, "mma8x5x");

    return 0;
}