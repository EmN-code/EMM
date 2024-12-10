#include <thread_arch.h>
#include <thread.h>
#include <periph_conf.h>
#include <inttypes.h>

#include "mma8x5x.h"
#include "xtimer.h"

#define SLEEP_INTERVAL (5U * 1000000U) // Sleep for 5 seconds (5,000,000 microseconds)

static mma8x5x_t dev;

int main(void) {
    // Initialize the MMA8x5x sensor with default parameters
    const mma8x5x_params_t params = {
        .i2c = I2C_DEV(0), // Assuming I2C_DEV(0) is used for MMA8x5x
        .addr = MMA8X5X_DEFAULT_ADDR 
    };
    
    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Initialization of MMA8X5X failed\n");
        return 1;
    }    

    while (1) {
        if (mma8x5x_is_ready(&dev)) {
            mma8x5x_measurement_t result;
            mma8x5x_read(&dev, &result);
            printf("Acceleration: x=%d, y=%d, z=%d\n", result.x_axis, result.y_axis, result.z_axis);
        }
        else {
            printf("Sensor not ready\n");
        }
        
        xtimer_usleep(SLEEP_INTERVAL);
    }
    
    return 0;
}