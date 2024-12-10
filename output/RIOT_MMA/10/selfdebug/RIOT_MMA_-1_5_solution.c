#include <xtimer.h>
#include <stdio.h>
#include <periph/i2c.h>

#define I2C_INTERFACE I2C_DEV(0)
#define I2C_ADDR     0x1D   // Default I2C address for MMA8x5x
#define READ_INTERVAL (5 * US_PER_SEC)

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    // Initialize I2C
    if (i2c_init(I2C_INTERFACE) != 0) {
        puts("I2C initialization failed");
        return 1;
    }

    // Parameters for MMA8x5x
    mma8x5x_params_t params = {
        .i2c = I2C_INTERFACE,
        .addr = I2C_ADDR,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    // Initialize MMA8x5x
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("MMA8x5x initialization failed");
        return 1;
    }

    // Main loop
    while (1) {
        // Read sensor data
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        }
        else {
            puts("Failed to read sensor data");
        }
        
        xtimer_sleep(5);  // Wait for 5 seconds
    }
    return 0;
}