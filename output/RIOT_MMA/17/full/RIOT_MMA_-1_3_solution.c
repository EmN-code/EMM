#include "nvs_flash.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "host/ble_ibeacon.h"
#include "event/thread.h"
#include "dht_params.h"
#include "thread_flags.h"
#include "thread.h"
#include "hashes/sha2xx_common.h"

#include <stdio.h>

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    /* Initialize the flash storage */
    nvs_flash_init();
    
    /* Set parameters for the accelerometer */
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,  // Correct the address definition here
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };
    
    /* Initialize the MMA8x5x sensor */
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Initialization failed\n");
        return 1;
    }

    while (1) {
        /* Read sensor data */
        mma8x5x_read(&dev, &data);
        
        /* Print acceleration data */
        printf("Acceleration: x=%d mg, y=%d mg, z=%d mg\n", data.x, data.y, data.z);
        
        /* Sleep for 5 seconds */
        xtimer_sleep(5);
    }

    return 0;
}
