#include <stdio.h>
#include <xtimer.h>
#include <periph/i2c.h>
#include <mma8x5x.h>

int main(void) {
    /* Initialize MMA8x5x device descriptor */
    mma8x5x_t dev;

    /* Configure parameters */
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = 0x1D,  // Adjust to your device's actual I2C address
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };
    
    /* Initialize I2C */
    if (i2c_init(params.i2c) != I2C_OK) {
        printf("Failed to initialize I2C\n");
        return 1;
    }

    /* Initialize MMA8x5x */
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("MMA8x5x initialization failed\n");
        return 1;
    }

    printf("MMA8x5x initialized successfully\n");

    /* Read accelerometer data every 5 seconds */
    mma8x5x_data_t data;

    while(1) {
        xtimer_sleep(5);

        mma8x5x_read(&dev, &data);

        printf("Acceleration - X: %d mg, Y: %d mg, Z: %d mg\n", 
               data.x, data.y, data.z);
    }

    return 0;
}