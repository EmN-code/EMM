
#include <unistd.h>  // For sleep function
#include "mma8x5x.h" // For MMA8x5x accelerometer

#define MMA8X5X_I2C_ADDRESS 0x1C // Define the I2C address for MMA8x5x

static mma8x5x_t dev;
mma8x5x_data_t data;

int main(void)
{
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        // Handle initialization error
        return 1;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        // Process the accelerometer data
        // For example, print the data
        printf("Acceleration: X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);

        sleep(5); // Sleep for 5 seconds
    }

    return 0;
}
