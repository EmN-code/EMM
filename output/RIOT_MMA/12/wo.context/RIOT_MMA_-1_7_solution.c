// This program initializes the MMA8x5x accelerometer and reads data every 5 seconds using RIOT OS on an ESP32.

#include "xtimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"

// Define the necessary parameters and variables
#define I2C_DEV            I2C_DEV(0)
#define MMA8X5X_ADDR       0x1D  // replace with actual address if needed
#define MMA8X5X_RATE       MMA8X5X_RATE_50HZ
#define MMA8X5X_RANGE      MMA8X5X_RANGE_2G
#define READ_INTERVAL      (5U * US_PER_SEC)  // 5 seconds interval

static mma8x5x_t dev;
static mma8x5x_data_t data;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV,
    .addr = MMA8X5X_ADDR,
    .rate = MMA8X5X_RATE,
    .range = MMA8X5X_RANGE
};

int main(void) {
    // Initialize the I2C interface and the accelerometer
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Failed to initialize the MMA8x5x device");
        return 1;
    }

    while (1) {
        // Read data from MMA8x5x
        if (mma8x5x_read(&dev, &data) != MMA8X5X_OK) {
            puts("Failed to read data from MMA8x5x");
        } else {
            printf("Accelerometer Data - X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        }

        // Wait for the next interval
        xtimer_sleep(READ_INTERVAL);
    }

    return 0;
}