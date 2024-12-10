#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "stdio.h"
#include "periph/i2c.h"
#include "periph_conf.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "periph/rtc.h"
#include "mma8x5x_regs.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "event/thread.h"
#include "dht_params.h"
#include "periph/gpio.h"

#define MMA8X5X_I2C_ADDRESS (0x1D)
#define STACK_SIZE (THREAD_STACKSIZE_MAIN)
#define PRIORITY (THREAD_PRIORITY_MAIN)

static char stack[STACK_SIZE];
static kernel_pid_t thread_pid;
static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void *mma8x5x_thread(void *arg) {
    (void)arg;
    mma8x5x_data_t data;
    int rc;
    while (1) {
        // Read accelerometer data
        rc = mma8x5x_read(&dev, &data);
        if (rc != MMA8X5X_OK) {
            printf("Read failed with error code %d\n", rc);
        } else {
            printf("Accel Data: x=%d y=%d z=%d\n", data.x, data.y, data.z);
        }
        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_SEC, 5);
    }
    return NULL;
}

int main(void) {
    // Initialize the MMA8x5x device
    int rc;
    while (1) {
        rc = mma8x5x_init(&dev, &params);
        if (rc != MMA8X5X_OK) {
            printf("Initialization failed with error %d\n", rc);
            xtimer_sleep(1);
        } else {
            printf("Initialization successful\n");
            break;
        }
    }

    // Create a thread that reads the accelerometer data
    thread_pid = thread_create(stack, sizeof(stack), PRIORITY,
                               THREAD_CREATE_STACKTEST, mma8x5x_thread,
                               NULL, "mma8x5x_thread");
    if (thread_pid < 0) {
        printf("Failed to create thread\n");
    }

    return 0;
}