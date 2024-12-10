#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "mymma8x5x.h" // Assume this is the header where CONFIG_MMA8X5X_I2C_ADDRESS is defined
#include "mma8x5x.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "MQTTClient.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "dht_params.h"
#include "event/thread.h"

#define STACK_SIZE (THREAD_STACKSIZE_MAIN)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

char stack[STACK_SIZE];

static void *mma8x5x_thread(void *arg) {
    (void)arg;
    mma8x5x_t dev;
    mma8x5x_data_t data;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };
    
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Error: Unable to initialize MMA8x5x\n");
        return NULL;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), PRIORITY, THREAD_CREATE_STACKTEST, mma8x5x_thread, NULL, "mma8x5x");
    return 0;
}