// Include the necessary header files
#include "thread.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "MQTTClient.h"
#include "periph_conf.h"
#include "event/thread.h"

// Define the device and parameters
static mma8x5x_t dev;
static mma8x5x_data_t data;

// Configuration parameters
mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

// Main function
int main(void)
{
    // Initialize device
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        // Initialization failed, handle error
        return 1;
    }

    while (1) {
        // Read data from the accelerometer
        mma8x5x_read(&dev, &data);

        // Process or print the data
        // printf("X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);

        // Sleep for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}