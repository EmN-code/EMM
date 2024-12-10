#include "xtimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"  // Include the library for the MMA8x5x sensor

#define I2C_BUS I2C_DEV(0) // Define the I2C bus
#define DELAY 5  // 5 seconds delay

static mma8x5x_t dev;
static mma8x5x_data_t data;

void read_mma8x5x_data(void) {
    // Read accelerometer data
    if (mma8x5x_read(&dev, &data) != MMA8X5X_OK) {
        printf("Failed to read data from MMA8x5x\n");
        return;
    }

    // Accessing the correct data fields of mma8x5x_data_t
    printf("Acceleration data: X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
}

int main(void) {
    // Initialize I2C
    i2c_init(I2C_BUS);

    // Set up the MMA8x5x sensor parameters
    mma8x5x_params_t params;
    params.i2c = I2C_BUS;
    params.addr = MMA8X5X_I2C_ADDRESS;
    params.rate = MMA8X5X_RATE_50HZ;
    params.range = MMA8X5X_RANGE_2G;

    // Initialize the sensor on the given parameters
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize the MMA8x5x sensor\n");
        return 1;
    }

    while (1) {
        read_mma8x5x_data();
        xtimer_sleep(DELAY);
    }

    return 0;
}