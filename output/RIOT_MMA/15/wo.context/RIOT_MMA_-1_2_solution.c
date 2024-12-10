#include thread.h
#include stdlib.h
#include inttypes.h
#include periph_conf.h

#include "mma8x5x.h"
#include "xtimer.h"

#define SLEEP_INTERVAL (5U * US_PER_SEC) // 5 seconds interval

static kernel_pid_t main_thread_pid;
static void *_thread_handler(void *arg);

int main(void) {
    main_thread_pid = thread_getpid();

    mma8x5x_t dev;
    mma8x5x_data_t data;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    while (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization of MMA8x5x failed, retrying...");
        xtimer_sleep(1);
    }

    puts("MMA8x5x initialized");

    char thread_stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, _thread_handler, NULL, "mma_thread");

    return 0;
}

static void *_thread_handler(void *arg) {
    mma8x5x_data_t data;
    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Accel (x, y, z): %d, %d, %d\n", data.x_axis, data.y_axis, data.z_axis);
        xtimer_sleep(SLEEP_INTERVAL);
    }
    return NULL;
}