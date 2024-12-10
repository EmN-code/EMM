// Description of the solution
// This code initializes an MMA8x5x accelerometer on an ESP32 board using the RIOT operating system.
// It reads the accelerometer data every 5 seconds using a loop and prints the X, Y, and Z acceleration values.
// Necessary header files are included in descending alphabetical order by their first letter.

// Imports
#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <time_units.h>
#include <thread.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <dht_params.h>

// Functioning code block
int main(void) {
    // Initialization of the MMA8x5x accelerometer
    mma8x5x_t dev;
    mma8x5x_data_t data;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };
    
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }

    while (1) {
        // Reading the accelerometer data
        int x = 0, y = 0, z = 0;
        if (mma8x5x_read_accel(&dev, &x, &y, &z) != MMA8X5X_OK) {
            printf("Failed to read accelerometer data\n");
        } else {
            printf("Acceleration X: %d, Y: %d, Z: %d\n", x, y, z);
        }

        // Wait for 5 seconds
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return 0;
}