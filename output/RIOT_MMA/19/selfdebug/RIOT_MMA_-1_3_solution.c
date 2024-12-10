#include "periph/i2c.h"
#include "xtimer.h"
#include "driver_mma8x5x.h"
#include "stdio.h"

#define MMA8X5X_I2C_ADDRESS (0x1D)  // Example I2C address
#define READ_INTERVAL (5U * 1000000U)  // 5 seconds in microseconds

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    puts("MMA8x5x Accelerometer Reading Example");
    
    // Initialize I2C device
    i2c_init(I2C_DEV(0));
    
    // Initialize the MMA8x5x sensor
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };
    int res = mma8x5x_init(&dev, &params);
    
    if (res != MMA8X5X_OK) {
        printf("Failed to initialize device: %d\n", res);
        return 1;
    }
    
    while (1) {
        // Read the accelerometer data
        mma8x5x_read(&dev, &data);
        printf("X: %d mg\tY: %d mg\tZ: %d mg\n", data.x, data.y, data.z);
        
        // Wait for the next read interval
        xtimer_usleep(READ_INTERVAL);
    }
    
    return 0;
}