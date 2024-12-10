#include <stdio.h>
#include <periph/i2c.h>
#include <mma8x5x.h>
#include <ztimer.h>

#define I2C_DEV I2C_DEV(0)
#define ADDR CONFIG_MMA8X5X_I2C_ADDRESS  // Corrected address macro name
#define INTERVAL (5U * US_PER_SEC)  // 5 seconds interval

int main(void) {
    mma8x5x_t dev;
    mma8x5x_data_t data;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV,
        .addr = ADDR,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    // Initialize the accelerometer
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Error: Unable to initialize the MMA8x5x sensor");
        return 1;
    }

    while (1) {
        // Read the accelerometer data
        mma8x5x_read(&dev, &data);

        // Output the accelerometer data
        printf("Acceleration - X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);

        // Sleep for INTERVAL duration
        ztimer_sleep(ZTIMER_SEC, INTERVAL / US_PER_SEC);
    }
    return 0;
}