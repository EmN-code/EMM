
#include <stdio.h>
#include <xtimer.h>
#include "mma8x5x.h"
#include "periph/i2c.h"

#define MMA8X5X_I2C_DEV         I2C_DEV(0)
#define MMA8X5X_I2C_ADDR        MMA8X5X_I2C_ADDR_DEFAULT
#define MMA8X5X_SAMPLE_RATE     MMA8X5X_RATE_1_5_HZ

mma8x5x_t dev;

int main(void)
{
    puts("MMA8x5x accelerometer driver test application");

    /* Initialize the I2C bus */
    i2c_init(MMA8X5X_I2C_DEV);

    /* Initialize the MMA8x5x sensor */
    if (mma8x5x_init(&dev, MMA8X5X_I2C_DEV, MMA8X5X_I2C_ADDR, MMA8X5X_RANGE_4G) != MMA8X5X_OK) {
        puts("Initialization of MMA8x5x sensor failed");
        return 1;
    }

    /* Set the sample rate */
    mma8x5x_set_rate(&dev, MMA8X5X_SAMPLE_RATE);

    while (1) {
        /* Read the accelerometer data */
        mma8x5x_data_t data;
        if (mma8x5x_read(&dev, &data) != MMA8X5X_OK) {
            puts("Failed to read accelerometer data");
        } else {
            printf("Accelerometer data - X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        }

        /* Wait for 5 seconds */
        xtimer_sleep(5);
    }

    return 0;
}