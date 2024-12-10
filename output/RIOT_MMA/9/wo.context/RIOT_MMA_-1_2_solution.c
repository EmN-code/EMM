#include "thread.h"
#include "string.h"
#include "mma8x5x.h"

#define INTERVAL (5U * 1000000U) /* interval for the periodic task, 5 seconds */

static kernel_pid_t _main_thread_pid;
static mma8x5x_t dev;
static mma8x5x_data_t data;

void *read_mma8x5x(void *arg){
    (void)arg;

    while (1) {
        int res = mma8x5x_read(&dev, &data);
        if (res != MMA8X5X_OK) {
            /* Handle read error */
            continue;
        }
        
        /* Process the data, e.g., print it */

        /* Wait 5 seconds */
        xtimer_usleep(INTERVAL);
    }

    return NULL;
}

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        /* Handle error */
        return 1;
    }

    _main_thread_pid = thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1,
                                     THREAD_CREATE_STACKTEST,
                                     read_mma8x5x, NULL, "mma8x5x_reader");

    return 0;
}