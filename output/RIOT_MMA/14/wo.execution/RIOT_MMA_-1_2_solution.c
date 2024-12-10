#include "thread.h"
#include "periph_conf.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "event/thread.h"

#define I2C_DEV(i)        (I2C_DEV_ ## i)
#define THREAD_PRIORITY   (THREAD_PRIORITY_MAIN - 1)

static kernel_pid_t main_thread_pid;

int main(void) {
    mma8x5x_t dev;
    mma8x5x_data_t data;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    main_thread_pid = thread_getpid();

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize mma8x5x\n");
        return 1;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("X: %d mg\n", data.x);
        printf("Y: %d mg\n", data.y);
        printf("Z: %d mg\n", data.z);

        i2c_acquire(I2C_DEV(0));
        thread_sleep(5 * US_PER_SEC);
        i2c_release(I2C_DEV(0));
    }

    return 0;
}