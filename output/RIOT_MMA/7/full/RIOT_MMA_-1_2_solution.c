#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "event/thread.h"
#include "xtimer.h"
#include "periph/i2c.h"
#include "periph/gpio.h"

#define I2C_DEV              I2C_DEV(0)
#define MMA8X5X_I2C_ADDRESS  (0x1D)
#define MMA8X5X_RATE_50HZ    (0)
#define MMA8X5X_RANGE_2G     (0)

int main(void) {
    mma8x5x_t dev;
    mma8x5x_data_t data;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV,
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G,
    };

    int res;
    while (1) {
        res = mma8x5x_init(&dev, &params);
        if (res == MMA8X5X_OK) {
            printf("MMA8x5x initialized successfully\n");
            break;
        } else {
            printf("Error initializing MMA8x5x: %d\n", res);
            xtimer_sleep(1);
        }
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
        xtimer_sleep(5);
    }

    return 0;
}
