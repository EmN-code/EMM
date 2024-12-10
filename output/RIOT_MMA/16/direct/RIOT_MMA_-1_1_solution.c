#include "ztimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"
#include "stdio.h"

#define I2C_DEV     0      // I2C device number
#define I2C_ADDR    0x1D   // MMA8x5x I2C address
#define READ_INTERVAL (5U * US_PER_SEC) // 5 seconds interval

static mma8x5x_t dev;

int main(void) {
    i2c_t i2c = I2C_DEV; // Initialize I2C device

    // Initialize MMA8x5x sensor with the I2C device and address
    if (mma8x5x_init(&dev, &i2c, I2C_ADDR, 0) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }

    while (1) {
        int16_t x, y, z;

        // Read accelerometer data from MMA8x5x
        if (mma8x5x_read(&dev, &x, &y, &z) == MMA8X5X_OK) {
            printf("Acceleration - X: %d, Y: %d, Z: %d\n", x, y, z);
        } else {
            printf("Failed to read data from MMA8x5x\n");
        }

        // Sleep for the defined interval
        ztimer_sleep(ZTIMER_SEC, READ_INTERVAL);
    }

    return 0;
}