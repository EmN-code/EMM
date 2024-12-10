#include "MQTTClient.h"
#include "evtimer_mbox.h"
#include "mma8x5x_regs.h"
#include "stdbool.h"
#include "ztimer.h"
#include "msg.h"
#include "dht_params.h"
#include "timex.h"
#include "stddef.h"
#include "stdio.h"
#include "time_units.h"
#include "stdlib.h"
#include "xtimer.h"
#include "mma8x5x_params.h"
#include "thread.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "unistd.h"
#include "event/thread.h"

#define I2C_DEV  I2C_DEV(0)
#define MMA8X5X_I2C_ADDRESS 0x1D
#define MMA8X5X_RATE_50HZ 0x02
#define MMA8X5X_RANGE_2G 0x00

static mma8x5x_t dev;
static mma8x5x_data_t data;

void *read_mma8x5x(void *arg) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV,
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    int init_status;
    while (1) {
        init_status = mma8x5x_init(&dev, &params);
        if (init_status == MMA8X5X_OK) {
            break;
        } else {
            printf("Failed to initialize MMA8x5x, retrying...\n");
            xtimer_sleep(1);
        }
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    thread_create("
    char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, read_mma8x5x, NULL, "mma8x5x_thread");
    return 0;
}