#include "thread.h"
#include "periph_conf.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "event/thread.h"
#include "periph/gpio.h"

#define I2C_DEV      I2C_DEV(0)
#define I2C_SPEED    I2C_SPEED_NORMAL
#define ADDR         MMA8X5X_I2C_ADDRESS
#define DELAY_SEC    5

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV,
        .addr = ADDR,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };
    
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Failed to initialize MMA8x5x!");
        return 1;
    }
    
    while (1) {
        /* Check if the device is ready */
        if (!mma8x5x_is_ready(&dev)) {
            puts("MMA8x5x not ready!");
        }
        else {
            mma8x5x_read(&dev, &data);
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        }
        
        /* Wait for 5 seconds */
        for (int i = 0; i < DELAY_SEC; i++) {
            sleep(1);
        }
    }
    return 0;
}