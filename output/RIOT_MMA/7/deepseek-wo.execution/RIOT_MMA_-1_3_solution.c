
#include <stdio.h>
#include <unistd.h> // For sleep function
#include "mma8x5x_params.h"
#include "evtimer_mbox.h"
#include "dht_params.h"
#include "mma8x5x_regs.h"
#include "ztimer.h"
#include "mma8x5x.h"
#include "evtimer_msg.h"
#include "xtimer.h"
#include "timex.h"
#include "mbox.h"
#include "event/thread.h"
#include "time_units.h"
#include "thread.h"

// Define the device and parameters
static mma8x5x_t dev;
mma8x5x_data_t data;

// Function to initialize the MMA8x5x accelerometer
int mma8x5x_init(mma8x5x_t *dev, const mma8x5x_params_t *params);

// Function to read accelerometer's data
void mma8x5x_read(const mma8x5x_t *dev, mma8x5x_data_t *data);

int main(void)
{
    // Initialize the MMA8x5x accelerometer
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x accelerometer\n");
        return 1;
    }

    while (1) {
        // Read the accelerometer data
        mma8x5x_read(&dev, &data);

        // Print the accelerometer data
        printf("Acceleration: X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);

        // Delay for 5 seconds
        sleep(5);
    }

    return 0;
}
