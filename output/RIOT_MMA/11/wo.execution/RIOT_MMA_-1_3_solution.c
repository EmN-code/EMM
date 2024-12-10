#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "periph_conf.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "event/thread.h"
#include "dht_params.h"

#define STACK_SIZE (THREAD_STACKSIZE_MAIN)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN)

static kernel_pid_t thread_pid;
static char stack[STACK_SIZE];

static void read_mma8x5x(void) {
    mma8x5x_t dev;
    mma8x5x_data_t data;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,  // Correct address setup
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };
    
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize sensor\n");
        return;
    }
    
    while (1) {
        mma8x5x_read(&dev, &data);
        printf("X = %d mg, Y = %d mg, Z = %d mg\n", data.x, data.y, data.z);
        xtimer_sleep(5);
    }
}

int main(void) {
    thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY, 0,
                               (thread_task_func_t)read_mma8x5x, NULL, "mma8x5x_reader");
    return 0;
}