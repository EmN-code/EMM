#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "thread_arch.h"
#include "thread.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "msg.h"
#include "mbox.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "architecture.h"

#define I2C_DEV       (I2C_DEV(0))
#define MMA8X5X_ADDR  (0x1D)
#define MMA8X5X_RATE  (50)
#define MMA8X5X_RANGE (2)

static mma8x5x_t dev;
static mma8x5x_data_t data;

void *sensor_thread(void *arg) {
    (void)arg;
    mma8x5x_params_t params;
    params.i2c = I2C_DEV;
    params.addr = MMA8X5X_ADDR;
    params.rate = MMA8X5X_RATE;
    params.range = MMA8X5X_RANGE;

    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Failed to initialize MMA8x5x.\n");
        return NULL;
    }

    while (1) {
        if (mma8x5x_is_ready(&dev)) {
            mma8x5x_read(&dev, &data);
            printf("Acceleration [mg]: X:%d, Y:%d, Z:%d\n", data.x_axis, data.y_axis, data.z_axis);
        }
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return NULL;
}

int main(void) {
    static char sensor_thread_stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(sensor_thread_stack, sizeof(sensor_thread_stack),
                  THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  sensor_thread, NULL, "sensor_thread");
    return 0;
}