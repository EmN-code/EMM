#include "xtimer.h"
#include "periph/i2c.h"
#include <stdio.h>

// Assuming that this address is correct for your setup
#define MMA8X5X_I2C_ADDRESS 0x1C  

typedef struct {
    i2c_t i2c;                  /**< I2C bus the device is connected to */
    uint8_t addr;               /**< I2C bus address of the device */
    uint8_t rate;               /**< sampling rate to use */
    uint8_t range;              /**< scale range to use */
    uint8_t offset[3];          /**< data offset in X, Y, and Z direction */
} mma8x5x_params_t;

static mma8x5x_t dev;
mma8x5x_data_t data;

mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void) {
    int init_status = mma8x5x_init(&dev, &params);
    if (init_status != MMA8X5X_OK) {
        printf("Initialization failed\n");
        return -1;
    }
    
    while (1) {
        // Simulating reading from the sensor
        // Replace with actual sensor read function
        int16_t x, y, z;
        mma8x5x_read_raw(&dev, &data);
        x = data.x;
        y = data.y;
        z = data.z;
        printf("X: %d, Y: %d, Z: %d\n", x, y, z);

        // Sleep for 5 seconds
        xtimer_sleep(5);
    }
    return 0;
}