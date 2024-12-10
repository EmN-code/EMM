#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "stddef.h"
#include "paho_mqtt.h"
#include "periph_conf.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "event/thread.h"
#include "dht_params.h"

#define I2C_DEV        I2C_DEV(0)
#define ADDR           MMA8X5X_I2C_ADDRESS
#define RATE           MMA8X5X_RATE_50HZ
#define RANGE          MMA8X5X_RANGE_2G
#define SLEEP_DURATION UINT32_C(5 * 1000000)

static kernel_pid_t thread_pid;
static char thread_stack[THREAD_STACKSIZE_MAIN];
static mma8x5x_t dev;

void *read_mma8x5x(void *arg) {
    (void)arg;
    mma8x5x_data_t data;
    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
        xtimer_usleep(SLEEP_DURATION);
    }
    return NULL;
}

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV,
        .addr = ADDR,
        .rate = RATE,
        .range = RANGE
    };
    
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization error!");
        return 1;
    }
    
    thread_pid = thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1, 
                               THREAD_CREATE_STACKTEST, read_mma8x5x, NULL, "mma8x5x_thread");
    if (thread_pid <= KERNEL_PID_UNDEF) {
        puts("Failed to create read thread");
        return 1;
    }

    return 0;
}