#include <xtimer.h>
#include <stdio.h>
#include "mma8x5x.h"
#include "periph/i2c.h"

#define DELAY 5

static mma8x5x_t dev;

int main(void) {
    /* Initialize I2C */
    if (i2c_init(I2C_DEV(0)) != 0) {
        puts("Error initializing I2C");
        return -1;
    }
    
    /* Initialize the MMA8x5x sensor */
    if (mma8x5x_init(&dev, &mma8x5x_params[0]) != 0) {
        puts("Error initializing MMA8x5x sensor");
        return -1;
    }
    
    /* Main loop */
    while (1) {
        int16_t x, y, z;
        
        /* Read accelerometer data */
        if (mma8x5x_read(&dev, &x, &y, &z) != 0) {
            puts("Error reading data from MMA8x5x");
        } else {
            printf("X: %d, Y: %d, Z: %d\n", x, y, z);
        }
        
        /* Wait for the specified delay */
        xtimer_sleep(DELAY);
    }

    return 0;
}
