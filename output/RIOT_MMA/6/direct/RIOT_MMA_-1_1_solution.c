#include <xtimer.h>
#include <stdio.h>
#include <mma8x5x.h>
#include <periph/i2c.h>

#define I2C_NUM         I2C_DEV(0)
#define ADDR            MMA8X5X_ADDR(0)
#define READ_INTERVAL   (5U * US_PER_SEC)

static mma8x5x_t dev;

int main(void) {
    printf("RIOT Example: Read from MMA8x5x sensor\n");

    if (mma8x5x_init(&dev, &mma8x5x_params[0]) != 0) {
        printf("Initialization Failed\n");
        return 1;
    }

    while (1) {
        int16_t x, y, z;
        if (mma8x5x_read(&dev, &x, &y, &z) != 0) {
            printf("Failed to read data\n");
        } else {
            printf("Acceleration X: %d, Y: %d, Z: %d\n", x, y, z);
        }

        xtimer_sleep(5); // Sleep for 5 seconds
    }

    return 0;
}